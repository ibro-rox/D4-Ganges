// Code for entering PID constants values on putty and then
// step 1: get them ton display on screen 
// step 2: Once step 1 is done get it to display then figure out how to convert them to 8 bit values that show a resolution between 0 and 1
// and display them
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
//#include "debug.h"
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

/*Includes usart.h header file which defines different functions for USART. USART header file version is 1.1*/

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
// transmit data function to transmit to the screen 
void uart_transmit( char data)
{
	while(!( UCSR0A &  _BV(UDRE0) ) ); //  data register enable bit is 1 if tx buffer is empy
	// if its 1 we load data onto UDR- Uart Data Register(buffer)
	UDR0 = data;
}
// reveive data function to receive values entered on screen
char uart_receive()
{
	while( !(UCSR0A &  _BV(RXC0) ) ); // if there is undread data in reveive buffer
		// RXCn goes 1
     return UDR0;// send the character in URD;
}
void send_string(char *str)
{
	int i;
	for( i = 0; str[i]; i++) uart_transmit(str[i]);
}//***************
void receive_string(char *ch)
{
	int i;
	for (i = 0; i < 5; ++i)
		ch[i] = uart_receive();
	ch[5] = '\0';
}
int main()
{
	init_uart1();
	//init_debug_uart0();
	char ki[6];
	char ch[60];
	float f, f_temp;
	uint16_t ten_bit;
	while(1)
	{
		send_string("\n\r Enter a Ki value: ");
		receive_string(ki);
		//f = atof(ki);
		 f = atof(ki);	
	     //if (f < 10.23)
		 //{
			f_temp = f*100;
			ten_bit = (uint16_t) (f_temp+0.5);
			sprintf(ch,"\n\r 10-bit dec value: %d",ten_bit);
			send_string(ch);
		 //}
		// else
		// 	send_string("\n\rInvaid entry its beyond range");
		// //printf("\n Enter a Ki value: ");
		//scanf("%f",&f);

		// if(f<10.23)
		// {
		// 	f = atof(ki);
		// 	f_temp = f*100;
		// 	ten_bit = (uint16_t) f_temp;
		
		// 	printf("\nYou entered %f -> %u",f,ten_bit);
		// }

		//else	
		//	printf("Invalid Value input");
		//ki = uart_receive();// hoping to get this to wait for input
//		receive_string(ki);
		// convert string to float
		//float f = atof(ki);// converts it to float number 	
		
		
		//WARNINGGGGGGG!
		//DO A CHECK IF THE TEN_BIT VALUE IS IN RANGE 0-1023
		//****************
		/*****************/
		// convert the float to binary
		// assume the range is 0-10.23 (0-1023 resolution)	
		//sprintf(ch,"  Ki in 10-bit decimal u entered is: %d",ten_bit);
		//send_string(ch);
	
	}
}

