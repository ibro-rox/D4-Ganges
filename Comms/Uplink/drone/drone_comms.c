#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "rfm12.h"
#include "drone_comms.h"

int main(void)
{	
	// Initialise rfm12 and interrupts
	rfm12_init();
	sei();

	uint8_t receivedpackettype;
	uint16_t receiveddata;

	while (1)
	{
		rfm12_tick();
		
		// Wait for data to be fully received
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{	
			receivedpackettype = rfm12_rx_type();
			receiveddata = rfm12_rx_buffer();

			
			// Extract 10-bit ADC value and packet type from the received packet
			Decode_data(&receivedpackettype, &receiveddata);
		}
	}
}

void Retrieve_data(uint8_t* type, uint16_t* data)
{
	// Combine packet type and data into a single 16-bit int
	uint16_t totalpacket;
	totalpacket = type;
	totalpacket = (totalpacket << DATA_BIT_SIZE) + data;

	#if ENCRYPTION_ENABLED
		// Decrypt the received packet
		totalpacket = Decrypt_data(totalpacket);
	#endif // ENCRYPTION_ENABLED


	// Split the decrypted packet into the data and the packet type
	Decode_data(type, data, totalpacket);
}

uint16_t Decode_data(uint8_t* type, uint16_t* data, uint16_t totalpacket)
{
	// Get 10-bit data from the 16 bit
	*data = totalpacket & (uint16_t)1023;
	
	// Get packet type
	*type = (totalpacket >> DATA_BIT_SIZE);
}

uint16_t Decrypt_data(uint16_t packet)
{
	// Retrieve the encryption key
	uint8_t encryption_key;
	encryption_key = (packet >> (DATA_BIT_SIZE + COMMAND_BIT_SIZE));

	// Retrieve bits that are shifted out when the left shift is done
	uint8_t rotated_out_bits;
	rotated_out_bits = (packet >> (DATA_BIT_SIZE + COMMAND_BIT_SIZE - encryption_key));

	// Get completely rotated bits by adding the shifted out bits to the
	// original packet left-shifted by the required number of bits.
	// It is & with a sequence of 1s to remove the encryption key from the overall packet
	uint16_t decrypted_packet;
	decrypted_packet = (((packet << encryption_key) & (pow(2, DATA_BIT_SIZE + COMMAND_BIT_SIZE) - 1)) + rotated_out_bits;

	return decrypted_packet;
}

// Send relevant data to the control Il Matto
/*
void Send_controls()
{

}
*/