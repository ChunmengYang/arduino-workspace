int inputPin = 3;

uint8_t prev = 1;
int pre_time = 0;
int pre_pin_time = 0;

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
  int pin_time = millis();
  // 两个连续的读数间隔必须大于500毫秒
  if((pin_time - pre_pin_time) > 1000) {
    Serial.println("0");
    pre_pin_time = pin_time;
  }
  
  int sensorValue = digitalRead(inputPin);
  if(sensorValue == 0 && sensorValue != prev) {
    int time = millis();
    // 两个连续的读数间隔必须大于500毫秒
    if((time - pre_time) > 1000) {
      pre_time = time;
      Serial.println("1");
    }
  }
  prev = sensorValue;
}