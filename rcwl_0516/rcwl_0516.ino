#include <Wire.h>

#define SLAVE_ADDRESS 0x04
#define  ECHO_PIN 9

int pirValue;
void setup ()
{
  Serial.begin(9600);
  pinMode(ECHO_PIN, INPUT);
  Wire.begin(SLAVE_ADDRESS); // join i2c bus with address 0x04
  Wire.onRequest(requestEvent);
  Serial.println("setup\n");
}

void loop()
{
  delay(200);
  pirValue = digitalRead(ECHO_PIN);
  Serial.println(pirValue);
}

void requestEvent() {
  Wire.write(pirValue);
}
