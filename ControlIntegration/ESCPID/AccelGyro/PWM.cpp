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
               _BV(COM1A1)| /* A output enabled*/  
               _BV(COM1B1)| /* B output enabled*/
               _BV(COM1C1); /* C output enabled*/
    TCCR1B =  _BV(CS10)  | /* no prescaling */ 
              _BV(WGM12) | /* fast PWM/MAX */ 
              _BV(WGM13) ; /* fast PWM/MAX */           
    ICR1 = TIMER_TOP;
    /* TIMER 3 */
    DDRC |= _BV(PC6); /* PWM 3A out (pin 5 on pro micro)*/
    TCCR3A =  _BV(WGM31) | /* fast PWM/MAX */
              _BV(COM3A1); /* A output enabled*/
    TCCR3B =  _BV(CS30)  | /* no prescaling*/
              _BV(WGM32) | /* fast PWM/MAX */ 
              _BV(WGM33) ;
    ICR3 = TIMER_TOP;
    pwm_duty(LEFT_FRONT_MOTOR, MIN_THROTTLE_IN);
    pwm_duty(RIGHT_FRONT_MOTOR, MIN_THROTTLE_IN);
    pwm_duty(LEFT_REAR_MOTOR, MIN_THROTTLE_IN);
    pwm_duty(RIGHT_REAR_MOTOR, MIN_THROTTLE_IN);
}

void pwm_duty(uint8_t motor,float duty) 
{   //duty is currently in ms, we need to convert it to a value in the correct range. 
    //range ms: 1000-2000 , range registers: 2000-4000 therefore multiply by 2
    uint16_t intDuty;
    
    duty = duty*SPEED_TO_PWM_GAIN - ((SPEED_TO_PWM_GAIN*MIN_THROTTLE_IN)-PWM_DUTY_MIN);
    //Serial.println("   //check this is within the correct limits");
    if(duty>PWM_DUTY_MAX) duty = PWM_DUTY_MAX;
    else if(duty<PWM_DUTY_MIN) duty = PWM_DUTY_MIN;
    intDuty = (uint16_t)duty;
    #ifdef SERIAL_ENABLED
    Serial.print("duty: ");
    Serial.println(intDuty);
    #endif

    switch(motor)
    {
      case LEFT_FRONT_MOTOR:
        OCR1A = intDuty;
        break;
      case RIGHT_FRONT_MOTOR:
        OCR1B = intDuty;
        break;
      case LEFT_REAR_MOTOR:
        OCR1C = intDuty;
        break;
      case RIGHT_REAR_MOTOR:
        OCR3A = intDuty;
        break;
    }
}

void setMotors (float throttle, float yaw, float pitch, float roll)
{
  //reasons for these particular equations are given below
  pwm_duty(LEFT_FRONT_MOTOR,  (throttle - roll - pitch + yaw));
  pwm_duty(RIGHT_FRONT_MOTOR, (throttle + roll - pitch - yaw));
  pwm_duty(LEFT_REAR_MOTOR,   (throttle - roll + pitch - yaw));
  pwm_duty(RIGHT_REAR_MOTOR,  (throttle + roll + pitch + yaw));
}
/*
CW motors    A,C
CCW motors   D,B

           Front
           +1 pitch
          C   D
-1 roll    \-/     +1 roll   right
           /-\
          B   A
           -1 pitch

c = throttle - roll + pitch + yaw
d = throttle + roll + pitch - yaw
b = throttle - roll - pitch - yaw
a = throttle + roll - pitch + yaw
[1]http://robotics.stackexchange.com/questions/5116/
how-to-find-a-solution-for-quadcopter-pid-control
//seems to be inverted pitch so we changed it
  pwm_duty(LEFT_FRONT_MOTOR, (uint16_t)(throttle - roll + pitch + yaw));
  pwm_duty(RIGHT_FRONT_MOTOR, (uint16_t)(throttle + roll + pitch - yaw));
  pwm_duty(LEFT_REAR_MOTOR, (uint16_t)(throttle - roll - pitch - yaw));
  pwm_duty(RIGHT_REAR_MOTOR, (uint16_t)(throttle + roll - pitch + yaw));
*/
