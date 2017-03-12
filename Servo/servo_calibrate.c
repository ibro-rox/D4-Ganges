/*
*	Pointless little servo test program
*	Controlled by IR sensor
*	ADC pin - PA0
*	PWM pin - PD7
*	RXD pin - PD0 Orange
*	TXD pin - PD1 Yellow
*/



#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"

#define SERVO_PWM_DUTY_MAX 256
#define SERVO_PWM_DUTY_MIN 0
#define PWM_PRESCALER 8UL
#define PWM_FREQUENCY 50
#define PWM_OFFSET 0

void init_pwm(void);
void pwm_duty(uint8_t duty);

int main (void)
{

	init_pwm();
	init_debug_uart0();
	
	uint8_t pwm_value = 0;
	
	while(1)
	{
		pwm_value++;
        pwm_duty(pwm_value);
		_delay_ms(100);
        if(pwm_value>=255)
            pwm_value = 0;
	}
}



void init_pwm(void)
{
   	/* TIMER 2 */
    DDRD |= _BV(PD6); /* PWM out */
    DDRD |= _BV(PD7); /* inv. PWM out */
    

    TCCR2A = _BV(WGM20) | /* fast PWM/MAX */
	         _BV(COM2A1); /* An output */
    TCCR2B = _BV(CS21)  | 
             _BV(CS22);   /* 1/256 prescaling */
}

void pwm_duty(uint8_t duty)
{
	duty = duty > PWM_DUTY_MAX ? PWM_DUTY_MAX : duty;
	duty = duty < PWM_DUTY_MIN ? PWM_DUTY_MIN : duty;
    	printf("\nPWM=%3u  ==>  ", duty);  
	OCR2A = duty;
}