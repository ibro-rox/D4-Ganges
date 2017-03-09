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

String throttleString = "";         // a string to hold incoming data
String yawString      = ""; 
String pitchString      = ""; 
String rollString      = ""; 
int throttleInput = 0;
int yawInput = 0;
int pitchInput = 0;
int rollInput = 0;

boolean stringComplete = false;  // whether the string is complete
char packetFlag;

void pulse(void);
void serialEvent(void);
void printInt(void);

void setup() 
{
  delay(1000);
  //wait for controller input that isnt zero data
  

  Serial1.begin(57600);
  // reserve 4 bytes for the input strings:
  throttleString.reserve(4);
  yawString.reserve(4);
  pitchString.reserve(4);
  rollString.reserve(4);
  pinMode(6,OUTPUT);
  digitalWrite(6,LOW);
  Serial1.println("Setup Complete");
  Serial1.println("Waiting for non-zero throttle");
  while(throttleInput == 0)
  {
    pulse();
    serialEvent();
    delay(100);
  }
  Serial1.println("Non-zero throttle acquired, entering loop");
 
  
  init_pwm();
  //pwm_duty(thrust,yaw,pitch,roll);//change this to controller input 
} 

void loop() 
{
    serialEvent();
  // complete the string when a newline arrives:
  if (stringComplete) {
   printInt();
    // clear the string:
    stringComplete = false;
    delay(10);
    pulse();  
  } 
    
    //TXLED1;
      //get data from Ilmatto and output same to all motors
     pwm_duty((uint16_t)rawToThrottle(throttleInput),
              (uint16_t)rawToThrottle(yawInput),
              (uint16_t)rawToThrottle(pitchInput),
              (uint16_t)rawToThrottle(rollInput));
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

float rawToThrottle(int controlIn)
{
  float output = controlIn - 512;
  if(output > 4)
  { 
    output = output*7.8125+2000;
    Serial1.print(output);
    return output;
  }
  return 2000;
}

void printInt(void)
{
    Serial1.print("Packet Received:");
    Serial1.print("t");
    Serial1.print(throttleInput);
    Serial1.print("y");
    Serial1.print(yawInput);
    Serial1.print("p");
    Serial1.print(pitchInput);
    Serial1.print("r");
    Serial1.println(rollInput);
//    Serial1.print("\tt");
//    Serial1.print((int)rawToThrottle(throttleInput));
//    Serial1.print("y");
//    Serial1.print((int)rawToThrottle(yawInput));
//    Serial1.print("p");
//    Serial1.print((int)rawToThrottle(pitchInput));
//    Serial1.print("r");
//    Serial1.println((int)rawToThrottle(rollInput));
    
    
    return;
}


void serialEvent(void) {
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    // add it to the input string selected by setValue:
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') 
    {
      stringComplete = true;  
      throttleInput = throttleString.toInt();
      yawInput = yawString.toInt();
      pitchInput = pitchString.toInt();
      rollInput = rollString.toInt();
      throttleString = "";
      yawString = "";
      pitchString = "";
      rollString = "";
    }
    else
    {
      if(inChar == 't' ||inChar == 'y'||inChar == 'p'||inChar == 'r')
      {
        packetFlag= inChar;
      }
      else{
        switch (packetFlag) {
          case 't'://throttle
            throttleString += inChar;
            break;
          case 'y': //yaw
            yawString += inChar;
            break;
          case 'p': //pitch 
            pitchString += inChar;
            break;
          case 'r': //roll
            rollString += inChar;
            break;
        }
      }
    }
  }
  return;
}

void pulse(void)
{
  digitalWrite(6,HIGH);
  delayMicroseconds(1);
  digitalWrite(6,LOW);
  return;
}
