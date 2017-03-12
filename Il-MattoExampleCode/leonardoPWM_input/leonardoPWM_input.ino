#define TIMER_TOP 40000
//PWM DUTY for ESCs
#define PWM_DUTY_MIN 2000 //does this need to be 2020 when we have a big delay?
#define PWM_DUTY_MAX 4000



#define THROTTLE_ID 't'
#define YAW_ID 'y'
#define PITCH_ID 'p'
#define ROLL_ID 'r'

#define YAW_P_ID  'a'
#define YAW_I_ID  'b'
#define YAW_D_ID  'c'

#define PITCH_P_ID  'd'
#define PITCH_I_ID  'e'
#define PITCH_D_ID  'f'

#define ROLL_P_ID  'g'
#define ROLL_I_ID  'h'
#define ROLL_D_ID  'i'


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

//for the k values 
String yawKpString = "";
String yawKiString = "";
String yawKdString = "";

String pitchKpString = "";
String pitchKiString = "";
String pitchKdString = "";

String rollKpString = "";
String rollKiString = "";
String rollKdString = "";

boolean stringComplete = false;  // whether the string is complete
char packetFlag;

void pulse(void);
void Serial1Event(void);
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
    Serial1Event();
    delay(100);
  }
  Serial1.println("Non-zero throttle acquired, entering loop");
 
  
  init_pwm();
  //pwm_duty(thrust,yaw,pitch,roll);//change this to controller input 
} 

void loop() 
{
    Serial1Event();
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
    //Serial1.print(output);
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
//    Serial11.print("\tt");
//    Serial11.print((int)rawToThrottle(throttleInput));
//    Serial11.print("y");
//    Serial11.print((int)rawToThrottle(yawInput));
//    Serial11.print("p");
//    Serial11.print((int)rawToThrottle(pitchInput));
//    Serial11.print("r");
//    Serial11.println((int)rawToThrottle(rollInput));
    
    
    return;
}


void Serial1Event(void) {
    bool throttle_set = false;
    bool yaw_set = false;
    bool pitch_set = false;
    bool roll_set = false;
    bool yaw_p_set = false;
    bool yaw_i_set = false;
    bool yaw_d_set = false;
    bool pitch_p_set = false;
    bool pitch_i_set = false;
    bool pitch_d_set = false;
    bool roll_p_set = false;
    bool roll_i_set = false;
    bool roll_d_set = false;
    float yaw_kp;
    float yaw_ki;
    float yaw_kd;
    float pitch_kp;
    float pitch_ki;
    float pitch_kd;
    float roll_kp;
    float roll_ki;
    float roll_kd;
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    // add it to the input string selected by setValue:
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') 
    {
        stringComplete = true;  
        if(throttle_set)
        {
            throttleInput = throttleString.toInt();
            throttleString = "";
        }
        if(yaw_set)
        {
            yawInput = yawString.toInt();
            yawString = "";
        }
        if(pitch_set)
        {
            pitchInput = pitchString.toInt();
            pitchString = "";
        }
        if(roll_set)
        {
            rollInput = rollString.toInt();
            rollString = "";
        }
        if(yaw_p_set)
        {
            yaw_kp = yawKpString.toInt()/100;
            Serial1.print("yaw_kp:");
            Serial1.println(yaw_kp);
            yawKpString = "";
        }
        if(yaw_i_set)
        {
            yaw_ki = yawKiString.toInt()/100;
            Serial1.print("yaw_ki:");
            Serial1.println(yaw_ki);
            yawKiString = "";
        }
        if(yaw_d_set)
        {
            yaw_kd = yawKdString.toInt()/100;
            Serial1.print("yaw_kd:");
            Serial1.println(yaw_kd);
            yawKdString = "";
        }
        if(pitch_p_set)
        {
            pitch_kp = pitchKpString.toInt()/100;
            Serial1.print("pitch_kp:");
            Serial1.println(pitch_kp);
            pitchKpString = "";
        }
        if(pitch_i_set)
        {
            pitch_ki = pitchKiString.toInt()/100;
            Serial1.print("pitch_ki:");
           Serial1.println(pitch_ki);
           pitchKiString = "";
        }
        if(pitch_d_set)
        {
            pitch_kd =pitchKdString.toInt()/100;
            Serial1.print("pitch_kd:");
            Serial1.println(pitch_kd);
            pitchKdString = "";
        }
        if(roll_p_set)
        {
            roll_kp = rollKpString.toInt()/100;
            Serial1.print("roll_kp:");
           Serial1.println(roll_kp);
           rollKpString = "";
        }
        if(roll_i_set)
        {
            roll_ki = (float)rollKiString.toInt()/100;
            Serial1.print("roll_ki:"); 
           Serial1.println(roll_ki);
           rollKiString = "";
        }
        if(roll_d_set)
        {
            roll_kd =rollKdString.toInt()/100;
            Serial1.print("pitch_kd:");
            Serial1.println(roll_kd);
            rollKdString = "";
        }
    }
    else
    {
      if(inChar == THROTTLE_ID || inChar == YAW_ID || inChar == PITCH_ID || inChar == ROLL_ID 
        || inChar == YAW_P_ID|| inChar == YAW_I_ID|| inChar == YAW_D_ID 
        || inChar == PITCH_P_ID|| inChar == PITCH_I_ID|| inChar == PITCH_D_ID
        || inChar == ROLL_P_ID|| inChar == ROLL_I_ID|| inChar == ROLL_D_ID)
      {
        packetFlag= inChar;
      }
      else{
        switch (packetFlag) {
          case THROTTLE_ID://throttle
            throttleString += inChar;
            throttle_set = true;
            break;
          case YAW_ID: //yaw
            yawString += inChar;
            yaw_set = true;
            break;
          case PITCH_ID: //pitch
            pitchString += inChar;
            pitch_set = true; 
            break;
          case ROLL_ID: //roll
            rollString += inChar;
            roll_set = true;
            break;
          case YAW_P_ID:
            yawKpString += inChar;
            yaw_p_set = true;
            break;
          case YAW_I_ID:
            yawKiString += inChar;
            yaw_i_set = true;
            break;
          case YAW_D_ID:
            yawKdString += inChar;
            yaw_d_set = true;
            break;
          case PITCH_P_ID:
            pitchKdString += inChar;
            pitch_p_set = true;  
            break;
          case PITCH_I_ID:
            pitchKiString += inChar;
            pitch_i_set = true; 
            break;
          case PITCH_D_ID:
            pitchKdString += inChar;
            pitch_d_set = true;  
            break;
          case ROLL_P_ID:
            rollKpString += inChar;
            roll_p_set = true;  
            break;
          case ROLL_I_ID:
            rollKiString += inChar;
            roll_i_set = true;  

            break;
          case ROLL_D_ID:
            rollKdString += inChar;
            roll_d_set = true;  
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
