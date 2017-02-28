
#include "atmega_16.h"
#include "atmega_16_config.h"

#ifdef SERIAL_DEBUG 
//============================ SERIAL DEBUG ROUTINES =================================//
FILE uart_out = FDEV_SETUP_STREAM ( uart_print, NULL, _FDEV_SETUP_WRITE );

int uart_print ( char c, FILE *stream )
{
  if ( c == '\n' )
    uart_print ( '\r', stream );
  loop_until_bit_is_set ( UCSRA, UDRE );
  UDR = c;
  return 0;
}
//============================ SERIAL DEBUG ROUTINES =================================//
#endif

#ifdef USART 
//================================ USART ROUTINES ===================================//
void usart_init ( void )
{
	UCSRB |= ( 1 << RXCIE ) | ( 1 << RXEN ) | ( 1 << TXEN );   	// Turn on the transmission reception ..
								// circuitry and receiver interrupt
	UCSRC |= ( 1 << URSEL ) | ( 1 << UCSZ0 ) | ( 1 << UCSZ1 ); // Use 8-bit character sizes

	UBRRL = BAUD_PRESCALE; 	// Load lower 8-bits of the baud rate value..
				// into the low byte of the UBRR register
	UBRRH = ( BAUD_PRESCALE >> 8 ); 	// Load upper 8-bits of the baud rate value..
					// into the high byte of the UBRR register
	
	#ifdef SERIAL_DEBUG 
	stdout = &uart_out;
	#endif
}
 
void usart_putch ( unsigned char send )
{
	while ( ( UCSRA & ( 1 << UDRE ) ) == 0 ); // Do nothing until UDR is ready..
	// for more data to be written to it
	UDR = send; // Send the byte 
}
 
unsigned int usart_getch ( void )
{
	while ( ( UCSRA & ( 1 << RXC ) ) == 0 );
	// Do nothing until data have been received and is ready to be read from UDR
	return ( UDR ); // return the byte
}

void usart_send_string ( const char* data )
{
	for( ; *data; data ++ )
		usart_putch ( *data );
}
//================================ USART ROUTINES ===================================//
#endif


#ifdef LCD 
//================================= LCD ROUTINES ===================================//
void avr_lcd_port_init ( void )
{
	DDRA = 0xFF;
}

void avr_lcdcmd ( char cmdout )
{
	PORTA = cmdout;
	PORTA &= ~ ( 1 << rs );
	PORTA &= ~ ( 1 << rw );
	PORTA |= ( 1<< en );
	ms_delay ( 1 );
	PORTA &= ~ ( 1 << en );
}
 
void avr_lcddata ( char dataout )
{
	PORTA = dataout;
	PORTA |= ( 1 << rs );
	PORTA &= ~ ( 1 << rw );
	PORTA |= ( 1 << en );
	ms_delay ( 1 );
	PORTA &= ~ ( 1 << en );
}
//================================= LCD ROUTINES ===================================//
#endif

#ifdef SPI
//================================= SPI ROUTINES ===================================//
void spi_init ( void )
{
//master,little endian, SCK phase low, SCK idle low//
	DDRB  = 0xBF; 
	SPCR = 0x52; 
	SPSR = 0x00;

	ms_delay ( 10 );
}

void spi_init_2x ( void )
{
	SPCR = 0x50; 
	SPSR |= ( 1 << SPI2X );

	ms_delay ( 10 );
}

void spi_tx ( unsigned char data )
{
	SPDR = data;
	while ( ! (SPSR & ( 1 << SPIF ) ) );
}

unsigned char spi_rx ( void )
{
	SPDR = 0xff;
	while ( ! ( SPSR & ( 1 << SPIF ) ) );
	return SPDR;
}
//================================= SPI ROUTINES ===================================//
#endif

#ifdef SDSC
//================================= SDSC ROUTINES ==================================//
void sdsc_pin_assert ( void )
{
	PORTB &= ~0x02;
}

void sdsc_pin_deassert ( void )
{
	PORTB |= 0x02;
}
//================================= SDSC ROUTINES ==================================//
#endif

void ms_delay ( double delay )
{
	volatile int ms = delay;
	volatile int i;

	for ( ; ms; ms -- )
		for ( i = 0; i < 400; i ++ );
}

void us_delay ( double delay )
{
	volatile int us = delay;
	volatile int i;

	for ( ; us; us -- )
		for ( i = 0; i < 4; i ++ );
}
