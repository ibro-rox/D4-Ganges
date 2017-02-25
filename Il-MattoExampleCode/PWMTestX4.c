#include <avr/io.h>
#include <util/delay.h>
#define PWM_DUTY_MAX 240
void init_pwm(void);
void pwm_duty(uint8_t x0,uint8_t x1,uint8_t x2,uint8_t x3);

int main(void)
{
	/* set LED pin as an output */
	DDRB |= _BV(PINB7);
	int x0 = 0;
	int x1 = 0;
	int x2 = 0;
	int x3 = 0;
	init_pwm();
	/* forever loop (embedded programs never normally terminate) */
	for (;;) 
	{
		x0=(x0<PWM_DUTY_MAX) ? x0+1:0;
		x1=(x1<PWM_DUTY_MAX) ? x1+2:0;
		x2=(x2<PWM_DUTY_MAX) ? x2+3:0;
		x3=(x3<PWM_DUTY_MAX) ? x3+4:0;
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

void pwm_duty(uint8_t x0,uint8_t x1,uint8_t x2,uint8_t x3) 
{
    //x = (x > PWM_DUTY_MAX) ? PWM_DUTY_MAX : x;

    OCR2A = x0;
    OCR2B = x1;
    OCR0A = x2;
    OCR0B = x3;
}