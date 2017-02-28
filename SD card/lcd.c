
#include "lcd.h"

void lcd_string ( const char *data )
{
	for ( ; *data; data ++)
		dis_data ( *data );
}

void lcd_clear ( void )
{
	dis_cmd ( 0x01 );
	lcd -> delay_ms ( 10 );
}

void lcd_2nd_line ( void )
{
	dis_cmd ( 0xC0 );
	lcd -> delay_ms ( 1 );
}

void lcd_1st_line ( void )
{
	dis_cmd ( 0x80 );
	lcd -> delay_ms ( 1 );
}

void lcd_init ( void ) // fuction for intialize 
{
	lcd -> lcd_port_init ();
	dis_cmd ( 0x02 ); // to initialize LCD in 4-bit mode.
	dis_cmd ( 0x28 ); //to initialize LCD in 2 lines, 5X7 dots and 4bit mode.
	dis_cmd ( 0x0C );
	dis_cmd ( 0x06 );
	dis_cmd ( 0x80 );
	dis_cmd ( 0x01 );
	lcd -> delay_ms ( 500 );
}
 
void dis_cmd ( char cmd_value )
{
	char cmd_value1;
	cmd_value1 = cmd_value & 0xF0; //mask lower nibble because PA4-PA7 pins are used. 
	lcd -> lcdcmd ( cmd_value1 ); // send to LCD
 
	cmd_value1 = ( ( cmd_value <<4 ) & 0xF0 ); //shift 4-bit and mask
	lcd -> lcdcmd ( cmd_value1 ); // send to LCD
}
 
 
void dis_data ( char data_value )
{
	char data_value1;
	data_value1 = data_value & 0xF0;
	lcd -> lcddata ( data_value1 );
 
	data_value1 = ( ( data_value << 4 ) & 0xF0 );
	lcd -> lcddata ( data_value1 );
}
 


