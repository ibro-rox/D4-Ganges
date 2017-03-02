
#ifndef _LCD_H_
#define _LCD_H_

typedef struct
		{
			void ( *delay_ms ) ( double ms );
			void ( *lcd_port_init ) ( void );
			void ( *lcdcmd ) ( char cmdout );
			void ( *lcddata ) ( char dataout );
		}										lcd_interface;

lcd_interface *lcd;

void lcd_init ( void );
void dis_cmd ( char );
void dis_data ( char );
void lcd_clear ( void );
void lcd_2nd_line ( void );
void lcd_1st_line ( void );
void lcd_string ( const char *data );

#endif
