#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define SLAVE_ADDRESS 0x06
#define  ECHO_PIN 9

#define LED_PIN 8
#define NUMPIXELS 12
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned long start_time;
short pre_dist = 0;
short dist;
short touching = 0;
short clicked = 0;
char buf[2];

void setup ()
{
  Serial.begin(9600);
  Serial.println("initializing...");
  
  pinMode(ECHO_PIN, INPUT);

  pixels.begin();
  pixels.setBrightness(64);

  // join i2c bus with address SLAVE_ADDRESS
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  Serial.println("starting...");
}

void loop()
{
  delay(150);
  dist = digitalRead(ECHO_PIN);

  // 根据光电数据确定开关
  if (dist == 0 && pre_dist == 0) {
    if (touching == 0) {
      clicked = 1;
      touching = 1;
      Serial.println(clicked);
    }
    start_time = millis();
    dist = 1;
  } else {
    if ((millis() - start_time) > 800) {
      touching = 0;
      clicked = 0;
    }
  }
  pre_dist = dist;
  
}

void light_on() {
  pixels.clear(); 
  for (int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(150, 150, 0));  
  }
  pixels.show();
}

void light_of_flow(float d) {
  if (d > 0) {
    d /= NUMPIXELS;
    d *= 1000;

    for (int i = NUMPIXELS - 1; i >= 0; i--) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
      delay(d);
    }
  }
}

void light_of() {
  pixels.clear(); 
  for (int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));  
  }
  pixels.show();
}

// i2c从机接收数据
void receiveEvent(int howMany) {
  String str;
  while (Wire.available() > 0) {
    str = str + char(Wire.read());   
  }

  if (str == "") {
    return;
  }
  Serial.println(str);
  String sep = "-";
  int index;
  String temp;

  int countor = 0;
  int model = 0;
  float duration = 0;
  do {
    index = str.indexOf(sep);
    if (index != -1) {
        temp = str.substring(0, index);
        str = str.substring(index + sep.length(), str.length());
    } else {
       if(str.length() > 0)
        temp = str;
    }
    if (countor == 0) {
      model = temp.toInt();
    }
    if (countor == 1) {
      char floatbuf[32];
      temp.toCharArray(floatbuf, sizeof(floatbuf));
      duration = atof(floatbuf);
    } 

    countor += 1;
  } while(index >=0);

  if (model == 0) {
    light_of();
  } else {
    light_on();
  }

  if (model == 7 || model == 8) {
    light_of_flow(duration);
  }
}
// i2c从机发送数据
void requestEvent() {
  // Write short(16bit) number
  buf[0] = byte(clicked);
  buf[1] = byte(clicked >> 8);
  Wire.write(buf, 2);

  clicked = 0;
}
