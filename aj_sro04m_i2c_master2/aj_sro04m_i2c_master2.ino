#include <Wire.h>

short addresses[5] = {3, 4, 5, 6, 7};
short pre_values[5] = {0, 0, 0, 0, 0};
unsigned long start_time;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Wire.begin(); 
}

void loop() {
  short values[5];
  
  for (short i = 0; i < 5; i++) {
    short address = addresses[i];
    // put your main code here, to run repeatedly:
    Wire.requestFrom(address, 2);    // request 2 bytes from slave device address

    short la = 0, lb = 0, lc = 0;
    while (Wire.available()) {  // slave may send less than requested
      char c = Wire.read();     // receive a byte as character
      if (lc == 0) {
        la = short(c);
      } else if (lc == 1) {
        lb = short(c);
      } else {
        break;
      }
      lc++;
    }

    la = la | 0xFF00;
    lb = (lb << 8) | 0xFF;
    short v = la & lb;
    
    if (v <= 30) {
      values[i] = v;
    } else {
      values[i] = 0;
    }
  }

  String result = "";
  bool all_is_0 = true;
  for (short i = 0; i < 5; i++) {
    if (values[i] > 0) {
      all_is_0 = false;
    }
    char s[2];
    itoa(values[i], s, 10);
    
    result = result + s;
    if (i < 4) {
       result = result + ',';
     }
  }
  Serial.println(result);
  delay(200);

  if (!all_is_0) {
    // 与上次数据比较是否有变化
    bool change = false;
    for (short i = 0; i < 5; i++) {
      if (pre_values[i] != values[i]) {
        change = true;
        pre_values[i] = values[i];
      }
    }
  
    if (change) {
      start_time = millis();
    } else {
      // 60秒无操作，停止互动
      if ((millis() - start_time) > 60000) {
        byte x = byte(1);
        for (short i = 0; i < 5; i++) {
          short address = addresses[i];
          Wire.beginTransmission(address);  // transmit to device #address
          Wire.write(x);                    // sends one byte
          Wire.endTransmission();           // stop transmitting
        }
      }
    }
  } else {
    for (short i = 0; i < 5; i++) {
      pre_values[i] = values[i];
    }
  }
}
