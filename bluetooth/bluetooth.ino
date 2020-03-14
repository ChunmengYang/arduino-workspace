#include <SoftwareSerial.h>

int Pin = 3;
uint8_t prev = 1;
int pre_time = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("AT+NAME=Texhong");
  delay(500);
  Serial.println("AT+ROLE=1");
  delay(500);
  Serial.println("AT+PSWD=1234");
  delay(500);
  
  // 3号数字口设置为输入状态
  pinMode(Pin, INPUT);
}

void loop() {
//  delay(1000);
//  Serial.println("1");
  
  int sensorValue = digitalRead(Pin);
  if(sensorValue == 0 && sensorValue != prev) {
    int time = millis();
    // 两个连续的读数间隔必须大于100毫秒
    if((time - pre_time) > 100) {
      pre_time = time;
      Serial.println("1");
    }
  }
  prev = sensorValue;
}
