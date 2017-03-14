#include <avr/io.h>

#include <avr/interrupt.h>

#include <util/delay.h>

#include <math.h>



#include "rfm12.h"

#include "basestation_comms.h"



#if ENABLE_ENCRYPTION

volatile uint8_t encryption_key;

#endif



// initialise adc

void adc_init()//[1]

{

	// In ADCSRA Enable ADC (set ADEN) and prescaler of 64

	ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);

}



uint16_t adc_read(int n)//[1]
{
	ADMUX = n;// represents PA2

	// start conversion
	ADCSRA |= _BV(ADSC);

	// wait for conversion to complete
	//while(!(ADCSRA & _BV(ADIF))){};

	while(ADCSRA & _BV(ADSC));
	ADC = (ADCH << 8) | ADCL;// [1]

	return (n == PIN_ROLL || n == PIN_YAW) ? 1023 - ADC : ADC;
}



void init_uart1()// initialize UART

{

	 //1. set the baud rate, lets configure to 9600;

	// set the baud rate registers Ref: [1],[2]

	UBRR0H = BAUDRATE >> 8;// UBRRnH is 8 bits left

	UBRR0L = BAUDRATE;



	 //2. setting up data packet: 8 bits ,no parity 1 stop bit

		// setting 8 bits got to UCSCR register Ref:[3], pg 185 of data sheet



	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01); // 8 bits, USBS1 = 0 for 1 stop bit



		// note: havnt set up the stop bit in Ref [2] slides

	// 3. from Ref[2] we now enable Transmission and receive n UCSRnB register

	UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);



}



// transmit data function

void uart_transmit( char data)

{

	while(!( UCSR0A &  _BV(UDRE0) ) ); //  data register enable bit is 1 if tx buffer is empy

	// if its 1 we load data onto UDR- Uart Data Register(buffer)

	UDR0 = data;

}



void send_string(char *str)

{

	int i;

	for( i = 0; str[i]; i++) uart_transmit(str[i]);

}



void Send_data(uint8_t type, uint16_t data)

{

	#if ENABLE_UPLINK

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
		char debug[30];
		// sprintf(debug,"\n\r Datatype: %u and packet: %u",type,datapacket);
		// send_string(debug);
		//char ch[100];

		//sprintf(ch, "\n\rSending: %u %u", type, datapacket);

		//send_string(ch);

		// Send packet to the buffer for transmission

		rfm12_tx(sizeof(datapacket), type, &datapacket);

	#endif

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
//********************************************************** adding the receive functions for downlink test 3
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

double to_distance(uint16_t adc_value)
{
	double distance, volts;
	volts = (adc_value*3.3) / 1024;
	distance = 24 / volts;

	return distance;
}