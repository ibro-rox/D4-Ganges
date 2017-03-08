/*
	basestation_comms.c
	Implements communications from ground to air on the basestation.
	Author: Joel Trickey
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#include "rfm12.h"
#include "basestation_comms.h"

volatile uint8_t encryption_key;

int main(void)
{
	// Initialise rfm12 and interrupts
	rfm12_init();
	sei();

	encryption_key = 5;

	// Send test data
	uint16_t testdata = 0;

	while (1)
	{
		rfm12_tick();
		
		#if UPLINK_TEST
			Send_data(OP_ROLL, testdata);
			testdata++;
			if (testdata == 1024) break;
			_delay_ms(1000);
		#endif
		
	}
	while (1) {};
}

//!
/*  
	Process data and send it to the transceiver for transmission. 
	If encryption is enabled in the basestation_comms.h then the data will be encrypted. 
	The 10-bit data is encoded such that the 2 MSBs are stored in the packet type.
*/
void Send_data(uint8_t type, uint16_t data)
{
	// Combine packet type and data into a single 16-bit int
	uint16_t totalpacket;
	totalpacket = type;
	totalpacket = (totalpacket << DATA_BIT_SIZE) + data;

	// Encrypt data
	#if ENCRYPTION_ENABLED
		totalpacket = Encrypt_data(totalpacket);
	#endif

	// Split 16-bit packet into two 8-bit ints - packet type and data
	uint8_t datapacket;
	Encode_data(&type, &datapacket, totalpacket);

	// Send packet to the buffer for transmission
	rfm12_tx(sizeof(datapacket), type, &datapacket);
}

//!
/*  
	Encode the total packet into the type and data
*/
void Encode_data(uint8_t* type, uint8_t* data, uint16_t totalpacket)
{
	// Data is equal to the 8 LSBs
	*data = totalpacket;

	// Type, encryption key and 2 bits of data are held in the 8 MSBs
	*type = (totalpacket >> DATA_BIT_SIZE);
}

//!
/*  
	Encrypt the packet type and data using an encryption key.
	This encryption key changes every time the data is encrypted.
*/
uint16_t Encrypt_data(uint16_t packet)
{
	// Retrieve bits that are shifted out when the right shift is done
	uint8_t rotated_out_bits;
	rotated_out_bits = (packet & ((uint8_t) pow(2, encryption_key) - 1));

	// Get completely rotated bits by adding the shifted out bits to the
	// original packet right-shifted by the required number of bits.
	uint16_t encrypted_packet;
	encrypted_packet = (packet >> encryption_key) + (rotated_out_bits << (COMMAND_BIT_SIZE + DATA_BIT_SIZE - encryption_key));

	// Add on the encryption key to the MSBs of the packet
	encrypted_packet = encrypted_packet + (encryption_key << (COMMAND_BIT_SIZE + DATA_BIT_SIZE));

	// Adjust encryption key for next transmission
	encryption_key = (encryption_key < 3) ? encryption_key + 5 : encryption_key - 3;
	if (encryption_key == 0) encryption_key = 5;

	return encrypted_packet;
}