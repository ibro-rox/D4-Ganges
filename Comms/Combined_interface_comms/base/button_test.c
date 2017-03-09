#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "basestation_comms.h"
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


int main(void)
{
	init_uart1();
	DDRB &= ~_BV(PB3);// enable input
	PORTB |= _BV(PB3);// enable pull ups
	char ch[40];
	// button states
	uint8_t button_present, button_previous;
	button_previous = 0;
	while(1)
	{
		//button_present = (PINC & _BV(PC3)) ? 1:0;
		if (PINB & _BV(PB3))
		{
			send_string("\n\r FUCK YES IT WORKS");
		}
			sprintf(ch,"\n\rbutton_present = %u", button_present);
			send_string(ch);
			if ((button_present == 1) && (button_previous == 0))
			{
				send_string("\n\r Rising edge");
				// Send_data(OP_BUTTON, BTN_CARGO_HOOK);
				// _delay_ms(2);
				// Send_data(OP_BUTTON, BTN_CARGO_HOOK);
				// _delay_ms(2);
				// rfm12_tick();

			}
			button_previous = button_present;
	}
	return 0;
}