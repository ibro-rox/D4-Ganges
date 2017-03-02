#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"
#define BAUD 9600
#include <util/setbaud.h>




int main()
{
	const uint8_t segments[10] = {0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6};
	uint8_t i;
	init_debug_uart0();
	
	
	for(;;)
	{
		for(i=0;i<10;i++)
		{
			printf("%d = 0x%x\n",i,segments[i]);
			_delay_ms(1000);
			
		}	
		 fprintf(stderr,"Count overflow\n\r");
	}
	

}