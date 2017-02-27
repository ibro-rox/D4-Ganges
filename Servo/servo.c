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

#define PWM_DUTY_MAX 240
#define PWM_DUTY_MIN 0
#define PWM_PRESCALER 8UL
#define PWM_FREQUENCY 50
#define PWM_OFFSET 0

void init_pwm(void);
void pwm_duty(uint8_t duty);

void init_adc(void);
uint16_t adc_read(void);

int main (void)
{

	init_pwm();
	init_adc();
	init_debug_uart0();
	
	uint16_t adc_value;	
	uint8_t pwm_value;
	
	while(1)
	{
		adc_value = adc_read();
		pwm_value = (uint8_t) (adc_value/4) + PWM_OFFSET;	
		pwm_duty(pwm_value);
		_delay_ms(100);
	}
}



void init_pwm(void)
{
   	/* TIMER 2 */
    DDRD |= _BV(PD6); /* PWM out */
    DDRD |= _BV(PD7); /* inv. PWM out */
    

    TCCR2A = _BV(WGM20) | /* fast PWM/MAX */
	     _BV(COM2A1); /* A output */
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

void init_adc (void)
{
    /* REFSx = 0 : Select AREF as reference
     * ADLAR = 0 : Right shift result
     *  MUXx = 0 : Default to channel 0
     */
    ADMUX = 0x00;
    /*  ADEN = 1 : Enable the ADC
     * ADPS2 = 1 : Configure ADC prescaler
     * ADPS1 = 1 : F_ADC = F_CPU / 64
     * ADPS0 = 0 :       = 187.5 kHz
     */
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
}


uint16_t adc_read(void)
{
     uint16_t adc_in;
         
     /* Start single conversion */
     ADCSRA |= _BV ( ADSC );
     /* Wait for conversion to complete */
     while ( ADCSRA & _BV ( ADSC ) );
     adc_in = ADC;
    
     printf("ADC=%4d", adc_in);  
 
return adc_in;
}