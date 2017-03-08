// This code involves taking a copy from drone_comms.c and simply printing off the received data off putty
// Arthur: Joel And Mohammed
// Date : 3/1/17
// this code is also for test 1 im mentioned in my pink book
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>// for sprintf()
#include <math.h>

#include "rfm12.h"
#include "drone_serial_comms.h"
#include "debug.h"

#ifdef BAUD
#include <util/setbaud.h>
#endif

volatile uint8_t serial_interrupt_flag = 0;

ISR(INT0_vect)
{
	serial_interrupt_flag = 1; 			// Interrupt sets flag could be replaced
}							// with longer ISR if immediately needed

int main(void)
{
	// Initialise rfm12 and interrupts
	rfm12_init(); 
	init_debug_uart0();

	DDRD |= _BV(PD2);			// Set PD2 to output
	PORTD &= ~_BV(PD2);
	EICRA |= _BV(ISC01) | _BV(ISC00); 	// Set to trigger on rising edge
	EIMSK |= _BV(INT0); 			// Enable interrupt 
	sei();

	struct euler_angles desired_values;

	uint8_t receivedpackettype;
	uint16_t receiveddata;
	
	// Counter 
	uint16_t counter;
	counter = 0;

	while (1)
	{
		rfm12_tick();

		// Wait for data to be fully received
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{
			counter++;
			// Get the received packet type and data
			receivedpackettype = rfm12_rx_type();
			receiveddata = *rfm12_rx_buffer();
			rfm12_rx_clear();

			// Decrypt (if enabled) and extract 10-bit data and packet type from the received packet 
			Retrieve_data(&receivedpackettype, &receiveddata);

#if UPLINK_TEST
			sprintf(ch, "\n\rReceived: %u %u", receivedpackettype, receiveddata);
			send_string(ch);
			if (receiveddata > 1000) break;
#endif
			// Assign received data to the desired_values struct
#if ENABLE_CONTROLS
			switch (receivedpackettype)
			{
			case OP_THRUST:
				desired_values.throttle = receiveddata;
				break;
			case OP_ROLL:
				break;
				desired_values.roll = receiveddata;
			case OP_YAW:
				desired_values.yaw = receiveddata;
				break;
			case OP_PITCH:
				desired_values.pitch = receiveddata;
				break;
			case OP_BUTTON:
				switch (receiveddata)
				{
				case BTN_CARGO_HOOK_UP:
					// Set cargo hook up
					break;
				case BTN_CARGO_HOOK_DOWN:
					// Set cargo hook down
					break;
				case BTN_FLIGHT_MODE:

					break;
				case BTN_PID_MODE:

					break;
				}
				break;
			}
#endif
		}

		if (counter >= 150)
		{
			Send_data(OP_THRUST, receiveddata);
			_delay_ms(2);
			rfm12_tick();
			Send_data(OP_THRUST, receiveddata);
			_delay_ms(2);
			rfm12_tick();
			counter = 0;
		}
		
		if (serial_interrupt_flag)
			send_packet(&desired_values);
	}
}

void send_packet(struct euler_angles *input)
{
	if (is_valid_packet(input))
		printf("t%dy%dp%dr%d\n", input->throttle, input->yaw, input->pitch, input->roll);
	else
		printf("t%dy%dp%dr%d\n", 0, 0, 0, 0);

	serial_interrupt_flag = 0;
}

uint8_t is_valid_packet(struct euler_angles *input)
{
	return ((input->throttle <= 1023) && (input->yaw <= 1023) &&
		(input->pitch <= 1023) && (input->roll <= 1023));
}


void Retrieve_data(uint8_t* type, uint16_t* data)
{
	// Combine packet type and data into a single 16-bit int
	uint16_t totalpacket;
	totalpacket = *type;
	totalpacket = (totalpacket << 8) + *data;
#if ENABLE_ENCRYPTION
	// Decrypt the received packet
	totalpacket = Decrypt_data(totalpacket);
#endif

	// Split the decrypted packet into the data and the packet type
	Decode_data(type, data, totalpacket);
}

void Decode_data(uint8_t* type, uint16_t* data, uint16_t totalpacket)
{
	// Get 10-bit data from the 16 bit packet
	*data = totalpacket & (uint16_t)1023;

	// Get packet type
	*type = (totalpacket >> DATA_BIT_SIZE);
}

#if ENABLE_ENCRYPTION
uint16_t Decrypt_data(uint16_t packet)
{
	// Get the encryption key from the first 3 bits of the packet
	uint8_t encryption_key;
	encryption_key = (packet & 7);

	// Remove the encryption key from the packet
	packet = (packet & 65528);

	// Retrieve bits that are shifted out when the left shift is done
	uint8_t rotated_out_bits;
	rotated_out_bits = (packet >> (16 - encryption_key)) & get_1s(encryption_key);

	// Left shift packet and add on rotated-out bits in their previous position
	uint16_t decrypted_packet;
	decrypted_packet = (packet << encryption_key) + (rotated_out_bits << 3);

	// Remove any values from the first 3 bits and add on the encryption key
	decrypted_packet = (decrypted_packet & 65528) + encryption_key;

	return decrypted_packet;
}

uint16_t get_1s(uint8_t num)
{
	// Generate a variable that is num 1's
	uint16_t out;
	out = 1;

	uint8_t i;
	for (i = 1; i < num; i++)
	{
		out = (out << 1);
		out++;
	}

	return out;
}
#endif

void Send_data(uint8_t type, uint16_t data)
{
	// Combine packet type and data into a single 16-bit int
	uint16_t totalpacket;
	totalpacket = type;
	totalpacket = (totalpacket << DATA_BIT_SIZE) + data;

	// Encrypt data
#if ENABLE_ENCRYPTION
	totalpacket = Encrypt_data(totalpacket);
#endif
	// Split 16-bit packet into two 8-bit ints - packet type and data
	uint8_t datapacket;
	Encode_data(&type, &datapacket, totalpacket);

	//char ch[100];
	//sprintf(ch, "\n\rSending: %u %u", type, datapacket);
	//send_string(ch);
	// Send packet to the buffer for transmission
	rfm12_tx(sizeof(datapacket), type, &datapacket);
}



void Encode_data(uint8_t* type, uint8_t* data, uint16_t totalpacket)
{
	// Data is equal to the 8 LSBs
	*data = totalpacket;

	// Type, encryption key and 2 bits of data are held in the 8 MSBs
	*type = (totalpacket >> 8);
}



#if ENABLE_ENCRYPTION
uint16_t Encrypt_data(uint16_t packet)
{
	// Get the encryption key from the first 3 bits of the packet
	uint8_t encryption_key;
	encryption_key = (packet & 7);

	// Retrieve bits that are shifted out when the right shift is done
	uint8_t rotated_out_bits;
	rotated_out_bits = (packet >> 3) & get_1s(encryption_key);

	// Right shift packet and add on rotated-out bits in their new position
	uint16_t encrypted_packet;
	encrypted_packet = (packet >> encryption_key) + (rotated_out_bits << (13 + 3 - encryption_key));

	// Remove any values from the first 3 bits and add on the encryption key
	encrypted_packet = (encrypted_packet & 65528) + encryption_key;

	return encrypted_packet;
}

#endif