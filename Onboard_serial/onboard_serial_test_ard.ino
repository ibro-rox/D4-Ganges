String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(7,OUTPUT);
  digitalWrite(7,LOW);
}

void loop() {

  digitalWrite(7,HIGH);
  serialEvent();
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}


void serialEvent(void) {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') 
      stringComplete = true;      
  }
}






/*int incomingByte = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(13,OUTPUT);
  pinMode(7, OUTPUT);
  
  digitalWrite(13,LOW);
  digitalWrite(7,LOW);
}

void loop()
{

  while((send_request()))
    Serial.println(Serial.read());
 
    Serial.println("Timeout Error");

    

//  delay(10000);
  
  
//  digitalWrite(13,LOW);
  
}

boolean send_request(void)
{
  int i = 1;
  pinMode(7,OUTPUT);
  digitalWrite(7,HIGH);

  while(!(Serial.available() > 0))
  { 
    i++;
    if(i>10000) 
    { 
      digitalWrite(7,LOW);
      return false;
    }
  }
   digitalWrite(7,LOW);
   return true;
} */
  



