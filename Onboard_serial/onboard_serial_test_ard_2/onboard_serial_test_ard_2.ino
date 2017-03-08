String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void pulse(void);

void setup() {
  // initialize serial:
  //Serial.begin(57600);
  Serial1.begin(57600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(7,OUTPUT);
  digitalWrite(7,LOW);
  pulse();

}

void loop() {

  
  serialEvent();
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial1.print("Packet Received:");
    Serial1.print(inputString);
    // clear the string:
    inputString = "";
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
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') 
      stringComplete = true;      
  }
}

void pulse(void)
{
  digitalWrite(7,HIGH);
  delayMicroseconds(1);
  digitalWrite(7,LOW);
}

