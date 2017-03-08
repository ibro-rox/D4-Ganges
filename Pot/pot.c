#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "debug.h"

// avr-gcc -mmcu=atmega644p -DF_CPU=12000000 -Wall -Os -Wl,-u,vfprintf -lprintf_flt -lm pot.c -o pot.elf
// avr-objcopy -O ihex pot.elf pot.hex
// avrdude -c usbasp -p m644p -U flash:w:pot.hex

void init_adc(void)
{
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADEN);	//_BV(ADPS2) | _BV(ADPS1) |	
	ADMUX |= _BV(REFS0);	
}

uint16_t read_adc(uint8_t channel)
{	
	channel &= 0x07;
	ADMUX = (ADMUX & 0xF8)|channel;  
	ADCSRA |= _BV(ADSC);
	while(ADCSRA & _BV(ADSC));		
	return ADC;
}

int main(void)
{
	uint16_t result0, result1, result2, result3;
		
	init_debug_uart0();
	init_adc();
	
	for (;;) 
	{							
		result0 = read_adc(0);
		result1 = read_adc(1);
		result2 = read_adc(2);
		result3 = read_adc(3);
		
		printf("%d %d %d %d\n",result0,result1,result2,result3);
		//result0 = 0x0000;
		//result1 = 0x0000;
		//voltage = 0;
		
		_delay_ms(100);
	}
}