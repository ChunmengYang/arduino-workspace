#include <Wire.h>

short addresses[5] = {3, 4, 5, 6, 7};

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
  for (short i = 0; i < 5; i++) {
    char s[2];
    itoa(values[i], s, 10);
    
    result = result + s;
    if (i < 4) {
       result = result + ',';
     }
  }

  // 发送互动参数
  Serial.println(result);
  
  // 接收回传参数
  String str = "";
  while(Serial.available()){
    str = str + char(Serial.read());
  }
  Serial.println(str);

  if (str == "") {
    delay(200);
    return;
  }

  String sep = ",";
  int index; 
  String itemStr;
  do {
    index = str.indexOf(sep);
    if (index != -1) {
        itemStr = str.substring(0, index);
        str = str.substring(index + sep.length(), str.length());
    } else {
       if(str.length() > 0)
        itemStr = str;
    }

    String itemSep = "-";
    int i2cIndex = -1;
    String temps = "";
    int itemIndex = itemStr.indexOf(itemSep);
    if (itemIndex != -1) {
        i2cIndex = itemStr.substring(0, itemIndex).toInt();
        temps = itemStr.substring(itemIndex + itemSep.length(), itemStr.length());

        if (i2cIndex >= 0 && i2cIndex < 5) {
          short address = addresses[i2cIndex];
          Wire.beginTransmission(address);  // transmit to device #address
          Wire.write(temps.c_str());        // sends String
          Wire.endTransmission();   
        }
    }

  } while(index >=0);  
 
  delay(180);
}
