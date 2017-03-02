int incomingByte = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(13,OUTPUT);
  pinMode(7 OUTPUT);
  
  digitalWrite(13,LOW);
  digitalWrite(7,LOW);
}

void loop()
{

  digitalWrite(7,HIGH);
  if(Serial.available() > 0)
  {
    incomingByte = Serial.read();

    Serial.println(incomingByte, DEC);
    digitalWrite(13,HIGH);
  }

  delay(10000);
  
  
  digitalWrite(13,LOW);
  
}
