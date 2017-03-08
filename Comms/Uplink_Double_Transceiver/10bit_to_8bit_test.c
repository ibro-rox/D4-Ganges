#include <avr/io.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
	// Encode

	// 998 = 11 1110 0110
	uint16_t ADCoutput = 998;
	// 1110 0110
	uint8_t lsb8 = ADCoutput;
	// 11 0000 0000
	uint16_t msb2 = ADCOutput - lsb8;
	// 00 0000 0011
	msb2 = msb2 >> 8;
	// 0000 1011
	uint8_t packettype = (2 << 2) + msb2;

	// Sent data: [00001011] [1110 0110]  

	// Decode

	// 0000 1011 1110 0110
	uint16_t receiveddata = (packettype << 8) + lsb8;
	// 0000 0011 1110 0110
	uint16_t decodeddata = receiveddata & (uint16_t)1023;
	fprintf(stderr, "Decoded data: " + decodeddata);
	while (1) {};
}