String throttleString = "";         // a string to hold incoming data
String yawString      = ""; 
String pitchString      = ""; 
String rollString      = ""; 
int throttleInput;
int yawInput;
int pitchInput;
int rollInput;

boolean stringComplete = false;  // whether the string is complete
char setValue;

void pulse(void);

void setup() {
  // initialize serial:
  //Serial.begin(57600);
  Serial1.begin(57600);
  // reserve 200 bytes for the inputString:
  throttleString.reserve(4);
  yawString.reserve(4);
  pitchString.reserve(4);
  rollString.reserve(4);
  pinMode(7,OUTPUT);
  digitalWrite(7,LOW);
  pulse();
  Serial1.println("Starting program");
}

void loop() {
  serialEvent();
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial1.print("Packet Received:");
    Serial1.print("t");
    Serial1.print(throttleInput);
    Serial1.print("y");
    Serial1.print(yawInput);
    Serial1.print("p");
    Serial1.print(pitchInput);
    Serial1.print("r");
    Serial1.println(rollInput);
    // clear the string:
    stringComplete = false;
    delay(10);
    pulse();  
  }  
}


void serialEvent(void) {
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    // add it to the inputString:
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
        setValue = inChar;
      }
      else{
        switch (setValue) {
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
}

void pulse(void)
{
  digitalWrite(7,HIGH);
  delayMicroseconds(1);
  digitalWrite(7,LOW);
}

