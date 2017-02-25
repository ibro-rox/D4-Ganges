/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License. 
 *           View this license at http://creativecommons.org/about/licenses/
 *   Notes: F_CPU must be defined to match the clock frequency
 */
#include <avr/io.h>
#include <util/delay.h>
#define PWM_DUTY_MAX 240
void init_pwm(void);
void pwm_duty(uint8_t x0,uint8_t x1,uint8_t x2,uint8_t x3);

int main(void)
{
	/* set LED pin as an output */
	DDRB |= _BV(PINB7);
	int x0 = 5;
	int x1 = 50;
	int x2 = 125;
	int x3 = 240;
	init_pwm();
	/* forever loop (embedded programs never normally terminate) */
	for (;;) 
	{
		pwm_duty(x0,x1,x2,x3); 
		_delay_ms(10);
	}
}

void init_pwm(void)
{
    /* TIMER 2 */
    DDRD |= _BV(PD6); /* PWM out */
    DDRD |= _BV(PD7); /* inv. PWM out */
    

    TCCR2A = 	_BV(WGM20) | /* fast PWM/MAX */
	     		_BV(WGM21) | /* fast PWM/MAX */
	     		_BV(COM2A1)| /* A output */	
	     		_BV(COM2B1); /* B output */
    TCCR2B = 	_BV(CS22)  | /* /258 prescaling */
    			_BV(CS21)  ;   

    /* TIMER 0 */
    DDRB |= _BV(PB4); /* PWM out */
    DDRB |= _BV(PB3); /* inv. PWM out */
    

    TCCR0A = 	_BV(WGM00) | /* fast PWM/MAX */
	     		_BV(WGM01) | /* fast PWM/MAX */
	     		_BV(COM0A1)| /* A output */	
	     		_BV(COM0B1); /* B output */
    TCCR0B = 	_BV(CS02)  ; /* /258 prescaling */ 
}


/* Adjust PWM duty cycle
   Keep in mind this is not monotonic
   a 100% duty cycle has no switching
   and consequently will not boost.  
*/
void pwm_duty(uint8_t x0,uint8_t x1,uint8_t x2,uint8_t x3) 
{
    //x = (x > PWM_DUTY_MAX) ? PWM_DUTY_MAX : x;
    
    //printf(" PWM=%3u  ==>  ", x);  

    OCR2A = x0;
    OCR2B = x1;
    OCR0A = x2;
    OCR0B = x3;
}