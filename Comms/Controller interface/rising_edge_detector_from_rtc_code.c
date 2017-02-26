// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

// pin for clock
int clock_pin = 13;
// temperature sensor pin
int temp_pin = 0;
// present value and previous values of clock
bool clk_last = 0;
bool clk_now;
void setup () {

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);

  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void loop () {
    DateTime now = rtc.now();
    // reading ADC value from analogue pin for temp sensor
    int reading = analogRead(temp_pin);
    // convert the adc value to corresponding voltage
    float voltage = (reading * 3.3)/1024.0 ;
    // temperature
    float temp = (voltage - 0.5)*100;
    // reading present state/value from clock signal 
    clk_now = digitalRead(clock_pin);
    if (clk_now == 1 && clk_last == 0)
    {
      Serial.print("Temperature at Rising edge : ");
      Serial.print(temp);
      Serial.print(" At time: ");
       Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
      }
  
     
   // update last value with current value of clock
   clk_last = clk_now;

    Serial.println();
    // delay of half a second(T_clock = 1 second) to observe the output for test
    // remove this delay when using it
    delay(240);
    Serial.println();
   // delay(3000);
}