#define TIMER_TOP 40000
//PWM DUTY for ESCs
#define PWM_DUTY_MIN 2000 //does this need to be 2020 when we have a big delay?
#define PWM_DUTY_MAX 4000
void init_pwm(void);
void pwm_duty(uint16_t A,uint16_t B,uint16_t C,uint16_t D);
int thrust  = PWM_DUTY_MIN;
int yaw     = PWM_DUTY_MIN;
int pitch   = PWM_DUTY_MIN;
int roll    = PWM_DUTY_MIN;

void setup() 
{
  delay(1000);
  //wait for controller input that isnt zero data
  init_pwm();
  pwm_duty(thrust,yaw,pitch,roll);//change this to controller input 
} 

void loop() {
    //TXLED1;
      //get data from Ilmatto and output same to all motors
     pwm_duty(x1,x1,x2,x2);
     //delay(5);


}

void init_pwm(void)
{
    /* TIMER 1 */
    DDRB |= _BV(PB5); /* PWM 1A out (pin 9 on pro micro)*/
    DDRB |= _BV(PB6); /* PWM 1B out (pin 10 on pro micro)*/
    DDRB |= _BV(PB7); /* PWM 1C out (non existant on pro micro )*/
    ICR1 = TIMER_TOP;
    TCCR1A =   _BV(WGM11) | /* fast PWM/MAX */
               _BV(COM1A1)| /* A output enabled*/  
               _BV(COM1B1)| /* B output enabled*/
               _BV(COM1C1); /* C output enabled*/
    TCCR1B =  _BV(CS10)  | /*no prescalar*/
              //_BV(CS11)  | /* /8 prescaling */ 
              _BV(WGM12) | /* fast PWM/MAX */ 
              _BV(WGM13) ; /* fast PWM/MAX */           
    /* TIMER 3 */
    DDRC |= _BV(PC6); /* PWM 3A out (pin 5 on pro micro)*/
    TCCR3A =  _BV(WGM31) | /* fast PWM/MAX */
              _BV(COM3A1); /* A output enabled*/
    TCCR3B =  _BV(CS30)  | /*no prescalar*/
              //_BV(CS31)  | /* /8 prescaling */ 
              _BV(WGM32) | /* fast PWM/MAX */ 
              _BV(WGM33) ;
    ICR3 = TIMER_TOP;
}
void pwm_duty(uint16_t A,uint16_t B,uint16_t C,uint16_t D) 
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

int rawToThrottle(int controlIn)
{
  int output = controlIn - 512;
  if(output > 4)
  { 
    output = output*7.8125+2000;
    return output;
  }
  return 0;
}
