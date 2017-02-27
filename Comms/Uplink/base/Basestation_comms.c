#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#include "rfm12.h"
#include "basestation_comms.h"

uint8_t encrypt_key;

int main(void)
{
	// Initialise rfm12 and interrupts
	rfm12_init();
	sei();

	encrypt_key = 5;

	// Send test data
	uint16_t testdata = 0;
	Send_data(OP_THRUST, testdata);

	while (1)
	{
		rfm12_tick();
		
		// ### CODE FOR TEST ###
		testdata++;
		_delay_ms(1000);
		Send_data(OP_ROLL, testdata);
		// #######
	}
	
}

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

	// Split 16-bit packet into two 8-bit ints
	Encode_data(&type, &data, totalpacket);

	// Send packet to the buffer for transmission
	rfm12_tx(sizeof(data), type, &data);

}

void Encode_data(uint8_t* type, uint8_t* data, uint16_t totalpacket)
{
	/*
	// Get 8 LSBs from the 10-bit ADC output
	uint8_t lsb8;
	lsb8 = ADCOutput;
	*data = lsb8;

	// Get 2 MSBs from the 10-bit ADC output
	uint16_t msb2;
	msb2 = ADCOutput - lsb8;
	msb2 = msb2 >> 8;
	*/

	// Data is equal to the 8 LSBs
	*data = totalpacket;

	// Type is held in the 8 MSBs
	*type = (totalpacket >> DATA_BIT_SIZE);
}

uint16_t Encrypt_data(uint16_t packet)
{
	// Retrieve bits that are shifted out when the right shift is done
	uint8_t rotated_out_bits;
	rotated_out_bits = (packet & ((uint8_t) pow(2, encrypt_key) - 1));

	// Get completely rotated bits by adding the shifted out bits to the
	// original packet right-shifted by the required number of bits.
	uint16_t encrypted_packet; 
	encrypted_packet = (packet >> encrypt_key) + (rotated_out_bits << (COMMAND_BIT_SIZE + DATA_BIT_SIZE - encrypt_key));

	// Add on the encryption key to the MSBs of the packet
	encrypted_packet = encrypted_packet + (encrypt_key << COMMAND_BIT_SIZE + DATA_BIT_SIZE);
}