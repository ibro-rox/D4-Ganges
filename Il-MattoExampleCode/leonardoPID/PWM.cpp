#include "Definitions.h"
#include "PWM.h"
#include "Arduino.h"
void init_pwm(void)
{
    /* TIMER 1 */
    DDRB |= _BV(PB5); /* PWM 1A out (pin 9 on pro micro)*/
    DDRB |= _BV(PB6); /* PWM 1B out (pin 10 on pro micro)*/
    DDRB |= _BV(PB7); /* PWM 1C out (non existant on pro micro )*/
    TCCR1A =   _BV(WGM11) | /* fast PWM/MAX */
               _BV(WGM12) | /* fast PWM/MAX */
               _BV(WGM13) | /* fast PWM/MAX */
               _BV(COM1A1)| /* A output enabled*/  
               _BV(COM1B1)| /* C output enabled*/
               _BV(COM1C1); /* B output enabled*/
    TCCR0B =  _BV(CS11)   ; /* /8 prescaling */       
    ICR1 = TIMER_TOP;      
    /* TIMER 3 */
    DDRC |= _BV(PC6); /* PWM 3A out (pin 5 on pro micro)*/
    TCCR3A =  _BV(WGM31) | /* fast PWM/MAX */
              _BV(WGM32) | /* fast PWM/MAX */ 
              _BV(WGM33) |
              _BV(COM3A1); /* A output enabled*/
    TCCR3B =  _BV(CS31)  ; 
    ICR3 = TIMER_TOP;
}
void pwm_duty(uint8_t A,uint8_t B,uint8_t C,uint8_t D) 
{   //uncomment for limits
    /*if(A>PWM_DUTY_MAX) A = PWM_DUTY_MAX;
    else if(A<PWM_DUTY_MIN) A = PWM_DUTY_MIN;
    if(B>PWM_DUTY_MAX) B = PWM_DUTY_MAX;
    else if(B<PWM_DUTY_MIN) B = PWM_DUTY_MIN;
    if(C>PWM_DUTY_MAX) C = PWM_DUTY_MAX;
    else if(C<PWM_DUTY_MIN) C = PWM_DUTY_MIN;
    if(D>PWM_DUTY_MAX) D = PWM_DUTY_MAX;
    else if(D<PWM_DUTY_MIN) D = PWM_DUTY_MIN;*/
    OCR1A = A;
    OCR1B = B;
    OCR1C = C;
    OCR3A = D;
}