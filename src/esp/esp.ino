#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(13, 15, false); // d7 rx, d8 tx

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mySerial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly: 
  if (mySerial.available())
    Serial.println(mySerial.readStringUntil('\n'));
  // if (Serial.available())
  //   mySerial.wri3te(Serial.read());
}
