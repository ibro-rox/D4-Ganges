
#define F_CPU 8000000

#include <avr/io.h>
#include <stdlib.h>
#include "atmega_16.h"
#include "lcd.h"
#include "sdsc.h"
#include "fat32.h"

void init_devices ( void )
{
	lcd = ( lcd_interface * ) malloc ( sizeof ( lcd_interface ) );
	lcd -> delay_ms = ms_delay;
	lcd -> lcd_port_init = avr_lcd_port_init;
	lcd -> lcddata = avr_lcddata;
	lcd -> lcdcmd = avr_lcdcmd;
	lcd_init ();

	sdsc = ( sdsc_interface * ) malloc ( sizeof ( sdsc_interface ) );
	sdsc -> spi_tx = spi_tx;
	sdsc -> spi_rx = spi_rx;
	sdsc -> spi_init = spi_init;
	sdsc -> spi_init_2x = spi_init_2x;
	sdsc -> sdsc_assert = sdsc_pin_assert;
	sdsc -> sdsc_deassert = sdsc_pin_deassert;
	sdsc_init ();

	fat = ( fat32_interface * ) malloc ( sizeof ( fat32_interface ) );
	fat -> read_single_block = read_single_block;
	fat -> send_data_byte = usart_putch;
	fat32_init ();

	usart_init ();
}

int main ( void )
{
	char x;
	int i;
	char *p = "         ";

	init_devices ();
	
	lcd_string ( "    ENGINEERS   " );
	lcd_2nd_line ();
	lcd_string ( "     GARAGE     " );

	printf ( "\n======== ENGINEERS GARAGE SDCARD INTERFACING =======\n" );
	printf ( "\nSDSC card detected\n" );

MENU:
	printf ( "\nOPTIONS:\n" );
	printf ( "\n  1  : LIST ALL FILES\n" );
	printf ( "\n  2  : READ A FILE\n" );
	printf ( "\nEnter option : " );

	switch ( usart_getch () )
	{
		case '1':
			lcd_clear ();
			lcd_string ( "LISTING FILES ..." );
			list_files ();
			break;

		case '2':
			printf ( "\nEnter the file name : " );

			for ( i = 0; i < 9; i ++ )	
			{
				x = usart_getch ();
				usart_putch ( x );
				if ( '\r' == x )
					break;
				else
					p [ i ] = x;
			}
			p [ i ] = '\0';
			
			lcd_clear ();
			lcd_string ( "READING FILE ..." );
			read_file ( p );
	};
	
	goto MENU;

	while ( 1 )
	{
		;
	}

}

