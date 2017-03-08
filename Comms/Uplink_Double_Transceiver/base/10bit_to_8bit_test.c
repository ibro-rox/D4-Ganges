#include <avr/io.h>
#include <stdio.h>
#include <string.h>

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
}//***************

int main(void)
{
	init_uart1();
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
	while (1) {  };
}