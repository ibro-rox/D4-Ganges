
#ifndef _AVR_INTERFACE_
#define _AVR_INTERFACE_

#include <avr/io.h>
#include <stdio.h>

void ms_delay ( double delay );
void us_delay ( double delay );

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
void usart_init ( void );
void usart_putch ( unsigned char send );
unsigned int usart_getch ( void );
void usart_send_string ( const char* data );
int uart_print ( char c, FILE *stream );

void spi_init ( void );
void spi_init_2x ( void );
void spi_tx ( unsigned char data );
unsigned char spi_rx ( void );

void avr_lcd_port_init ( void );
void avr_lcdcmd ( char cmdout );
void avr_lcddata ( char dataout );

void sdsc_pin_assert ( void );
void sdsc_pin_deassert ( void );

#endif
