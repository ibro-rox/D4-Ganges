#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../comms.h"

// Mohammed's UART code
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

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
	UCSR0B = _BV(TXEN0) | _BV(RXEN0);
}

void uart_transmit(char data)
{
	while (!(UCSR0A &  _BV(UDRE0))); //  data register enable bit is 1 if tx buffer is empy
									 // if its 1 we load data onto UDR- Uart Data Register(buffer)
	UDR0 = data;
}

void send_string(char *str)
{
	int i;
	for (i = 0; str[i]; i++) uart_transmit(str[i]);
}//***************void test_encode_decode()

void test_encode()
{
	// Encode
	send_string("Encoding data\n");
	// 998 = 11 1110 0110
	uint16_t ADCoutput;
	ADCoutput = 998;
	// 1110 0110
	uint8_t lsb8;
	lsb8 = ADCoutput;
	// 11 0000 0000
	uint16_t msb2;
	msb2 = ADCoutput - lsb8;
	// 00 0000 0011
	msb2 = msb2 >> 8;
	// 0000 1011
	uint8_t packettype;
	packettype = (2 << 2) + msb2;

	// Sent data: [00001011] [1110 0110]  

	// Decode
	send_string("Decoding data\n");
	// 0000 1011 1110 0110
	uint16_t receiveddata;
	receiveddata = (packettype << 8) + lsb8;
	// 0000 0011 1110 0110
	uint16_t decodeddata;
	decodeddata = receiveddata & (uint16_t)1023;
	uint8_t decodedpackettype;
	decodedpackettype = (packettype >> 2);

	char sendData[30];
	sprintf(sendData, "Decoded data: %d\n", decodeddata);
	send_string(sendData);
}

uint16_t test_decrypt(uint16_t packet)
{
	// Retrieve the encryption key
	uint8_t encryption_key;
	encryption_key = (packet >> (DATA_BIT_SIZE + COMMAND_BIT_SIZE));

	char sendData0[30];
	sprintf(sendData0, "Encryption key: %u\r\n", encryption_key);
	send_string(sendData0);

	// Remove the encryption key from the packet
	packet = (packet & ((uint16_t)pow(2, DATA_BIT_SIZE + COMMAND_BIT_SIZE) - 1));

	// Retrieve bits that are shifted out when the left shift is done
	uint8_t rotated_out_bits;
	rotated_out_bits = (packet >> (DATA_BIT_SIZE + COMMAND_BIT_SIZE - encryption_key));

	// Get completely rotated bits by adding the shifted out bits to the
	// original packet left-shifted by the required number of bits.
	// It is & with a sequence of 1s to remove the encryption key from the overall packet
	uint16_t decrypted_packet;
	decrypted_packet = ((packet << encryption_key) & ((uint16_t)pow(2, DATA_BIT_SIZE + COMMAND_BIT_SIZE) - 1)) + rotated_out_bits;

	char sendData2[30];
	sprintf(sendData2, "Decrypted: %u\r\n", decrypted_packet);
	send_string(sendData2);

	return decrypted_packet;
}

uint16_t test_encrypt(uint16_t packet)
{
	//uint16_t packet;
	//packet = 343 + (6 << DATA_BIT_SIZE);

	char sendData1[50];
	sprintf(sendData1, "\r\nEncrypting %u\r\n", packet);
	send_string(sendData1);

	uint8_t encrypt_key;
	encrypt_key = 4;

	//send_string("Encrypting 868 with a packet of 2 by 2 bits\r\n");

	// Retrieve bits that are shifted out when the right shift is done
	uint8_t rotated_out_bits;
	rotated_out_bits = (packet & ((uint8_t)pow(2, encrypt_key) - 1));

	// Get completely rotated bits by adding the shifted out bits to the original packet right-shifted by the required number of bits.
	uint16_t encrypted_packet = (packet >> encrypt_key) + (rotated_out_bits << (COMMAND_BIT_SIZE + DATA_BIT_SIZE - encrypt_key));

	// Add on the encryption key to the MSBs of the packet
	encrypted_packet = encrypted_packet + (encrypt_key << (COMMAND_BIT_SIZE + DATA_BIT_SIZE));

	char sendData2[30];
	sprintf(sendData2, "Encrypted: %u\r\n", encrypted_packet);
	send_string(sendData2);

	return encrypted_packet;
}

int main(void)
{
	init_uart1();

	uint16_t testpacket, result;
	for (testpacket = 0; testpacket < 8192; testpacket++)
	{
		result = test_encrypt(testpacket);
		if (test_decrypt(result) != testpacket)
		{
			send_string("Error!");
			break;
		}
	}

	while (1) {};
}