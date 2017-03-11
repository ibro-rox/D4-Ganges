#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "debug.h"

// avr-gcc -mmcu=atmega644p -DF_CPU=12000000 -Wall -Os -Wl,-u,vfprintf -lprintf_flt -lm IR.c -o IR.elf
// avr-objcopy -O ihex IR.elf IR.hex
// avrdude -c usbasp -p m644p -U flash:w:IR.hex

void init_adc(void)
{
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADEN);		
	ADMUX |= _BV(REFS0);	
}

uint16_t read_adc(void)
{	
	ADCSRA |= _BV(ADSC);
	while(ADCSRA & _BV(ADSC));
		
	return ADC;
}

double to_distance(uint16_t adc_value)
{
	double distance,volts;
	volts = (adc_value*3.3)/1024; 
	distance = 24/volts;
	return distance;
}

int main(void)
{
	uint16_t result;
	double voltage;
	
	init_debug_uart0();
	init_adc();
	
	for (;;) 
	{					
		result = read_adc();
		
		voltage = to_distance(result);
		printf("%.6f\n",voltage);
		result = 0x0000;
		voltage = 0;
		
		_delay_ms(1000);
	}
}