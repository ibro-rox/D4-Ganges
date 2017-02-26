// Arthur: Mohammed Ibrahim
// Read 4 potentiometers which are 4 channels : THRUT, AILE, RUDD, ELEV
// Acknowledgement: [1] Had to re-write code from start for the adc functions 
// which were taken from Rhys thomas 
// Potential reason for my code not working : didnt do line 25
// here ADMUX = 2 represents PB3
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "rfm12.h"
uint16_t thrust,yaw,pitch,roll;
// initialzie adc
void adc_init()//[1]
{
	

	// In ADCSRA Enable ADC (set ADEN) and prescaler of 64
	ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
}
uint16_t adc_read(int n)//[1]
{
	ADMUX = n;// represents PA2
	// start conversion
	ADCSRA |= _BV(ADSC);
	// wait for conversion to complete
	//while(!(ADCSRA & _BV(ADIF))){};
	while(ADCSRA & _BV(ADSC));
	ADC = (ADCH << 8) | ADCL;// [1]
	return ADC;
}

void send_string(char *str)
{
	int i;
	for( i = 0; str[i]; i++) uart_transmit(str[i]);
}//***************
int main()
{
	adc_init();
	rfm12_init();// initialize rfm12 transceiver
	sei();// enable the ISR in the rfm12.h
	uint16_t throttle;
	char ch[20];
	while(1)// main forver loop
	{
		thrust = adc_read(0);// 10 bit value
		// split the 10 bit to 2 bits()
		// transmit it - rfrm12_tx() and afrm_tick()
		yaw = adc_read(1);
		// split the 10 bit to 2 bits()
		// transmit it - - rfrm12_tx() and afrm_tick()
		pitch = adc_read(2);
		// split the 10 bit to 2 bits()
		// transmit it - rfrm12_tx() and afrm_tick()
		roll = adc_read(3);
		// split the 10 bit to 2 bits()
		// transmit it - rfrm12_tx() and afrm_tick()
		//_delay_ms(100); ptoential delay to worry about ater 
	}
}