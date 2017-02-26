/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License. 
 *           View this license at http://creativecommons.org/about/licenses/
 *   Notes: F_CPU must be defined to match the clock frequency
 */
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	/* set LED pin as an output */
	DDRB |= _BV(PINB7);
	/* forever loop (embedded programs never normally terminate) */
	for (;;) 
	{
		/* Set pin B7 high */
		PORTB |= _BV(PINB7);
		_delay_ms(100);
		/* Set pin B7 low */
		PORTB &= ~_BV(PINB7);
		_delay_ms(900);
	}
}