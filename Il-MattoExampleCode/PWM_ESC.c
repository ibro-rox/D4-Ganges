#include <avr/io.h>
#include <util/delay.h>
#define PWM_DUTY_MAX 26
#define PWM_DUTY_MIN 13
void init_pwm(void);
void pwm_duty(uint8_t x0,uint8_t x1,uint8_t x2,uint8_t x3);

int main(void)
{
	int x0 = 0;
	int x1 = 0;
	int x2 = 0;
	int x3 = 0;
	init_pwm();

	for (;;) 
	{
		pwm_duty(x0,x1,x2,x3); 
		_delay_ms(10);
	}
}

void init_pwm(void)
{
    /* TIMER 0 */
    DDRB |= _BV(PB4); /* PWM 0A out */
    DDRB |= _BV(PB3); /* PWM 0B out */
    TCCR0A = 	_BV(WGM00) | /* fast PWM/MAX */
	     		_BV(WGM01) | /* fast PWM/MAX */
	     		_BV(COM0A1)| /* A output enabled*/	
	     		_BV(COM0B1); /* B output enabled*/
    TCCR0B = 	_BV(CS02)  | /* /1024 prescaling */ 
    			_BV(CS00)  ; //gives clock of 45.776Hz (we were aiming for 50Hz)
    /* TIMER 2 */
    DDRD |= _BV(PD6); /* PWM 2A out */
    DDRD |= _BV(PD7); /* PWM 2B out */
    TCCR2A = 	_BV(WGM20) | /* fast PWM/MAX */
	     		_BV(WGM21) | /* fast PWM/MAX */
	     		_BV(COM2A1)| /* A output enabled*/	
	     		_BV(COM2B1); /* B output enabled*/
    TCCR2B = 	_BV(CS22)  | /* 1024 prescaling */
    			_BV(CS21)  | //gives clock of 45.776Hz (we were aiming for 50Hz)
    			_BV(CS20)  ;   
}

void pwm_duty(uint8_t A,uint8_t B,uint8_t C,uint8_t D) 
{
    //A = (A > PWM_DUTY_MAX) ? PWM_DUTY_MAX : A;
    if(A>PWM_DUTY_MAX) A = PWM_DUTY_MAX;
    else if(A<PWM_DUTY_MIN) A = PWM_DUTY_MIN;
    if(B>PWM_DUTY_MAX) B = PWM_DUTY_MAX;
    else if(B<PWM_DUTY_MIN) B = PWM_DUTY_MIN;
    if(C>PWM_DUTY_MAX) C = PWM_DUTY_MAX;
    else if(C<PWM_DUTY_MIN) C = PWM_DUTY_MIN;
    if(D>PWM_DUTY_MAX) D = PWM_DUTY_MAX;
    else if(D<PWM_DUTY_MIN) D = PWM_DUTY_MIN;
    OCR2A = A;
    OCR2B = B;
    OCR0A = C;
    OCR0B = D;
}