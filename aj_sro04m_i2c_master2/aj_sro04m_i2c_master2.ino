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
  Serial.println(result);
  delay(200);
}
