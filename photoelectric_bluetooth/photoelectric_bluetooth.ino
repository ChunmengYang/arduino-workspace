int inputPin = 3;

uint8_t prev = 1;
int pre_time0 = 0;
int pre_time1 = 0;

void setup() {  
  // 3号数字口设置为输入状态
  pinMode(inputPin, INPUT);

  Serial.begin(9600);
  delay(200);
  Serial.println("AT");
  delay(200);
  Serial.println("AT+NAME=Texhong");
  delay(200);
  Serial.println("AT+ROLE=1");
  delay(200);
  Serial.println("AT+PSWD=1234");
  delay(200);
}

void loop() {
  int sensorValue = digitalRead(inputPin);
  if (sensorValue == 0) {
    int time = millis();
    if((time - pre_time0) > 300) {
      pre_time0 = time;
      Serial.println("0");
    }
  }
  
  if(sensorValue == 1 && sensorValue != prev) {
//    int time = millis();
//    // 两个连续的读数间隔必须大于300毫秒
//    if((time - pre_time1) > 300) {
//      pre_time1 = time;
//      Serial.println("1");
//    }
    Serial.println("1");
  }

  prev = sensorValue;
}
