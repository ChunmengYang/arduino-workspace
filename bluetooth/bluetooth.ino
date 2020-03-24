#include <SoftwareSerial.h>

//// software serial: RX = digital pin 8, TX = digital pin 9
//SoftwareSerial btSerial(8, 9);

int inputPin = 3;

uint8_t prev = 1;
int pre_time = 0;

void setup() {  
  // 3号数字口设置为输入状态
  pinMode(inputPin, INPUT);

  Serial.begin(9600);
  Serial.println("AT+NAME=Texhong");
  delay(500);
  Serial.println("AT+ROLE=1");
  delay(500);
  Serial.println("AT+PSWD=1234");
  delay(500);
}

void loop() {
  int sensorValue = digitalRead(inputPin);
  
  if(sensorValue == 0 && sensorValue != prev) {
    int time = millis();
    // 两个连续的读数间隔必须大于500毫秒
    if((time - pre_time) > 500) {
      pre_time = time;
      Serial.println("1");
    }
  }
  prev = sensorValue;
}
