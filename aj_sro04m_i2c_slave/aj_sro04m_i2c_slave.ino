#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define SLAVE_ADDRESS 0x03
#define  ECHO_PIN 9

#define LED_PIN 8
#define NUMPIXELS 12
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned long start_time;
short pre_dist = 0;
short dist;
short touching = 0;
short opened = 0;
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
  delay(200);
  dist = digitalRead(ECHO_PIN);
  Serial.println(dist);

  // 根据光电数据确定开关
  if (dist == 0 && pre_dist == 0) {
    if (touching == 0) {
      if (opened == 0) {
        opened = 1;
        light_on();
      } else {
        opened = 0;
        light_of();
      }
      touching = 1;
    }
    start_time = millis();
    dist = 1;
  } else {
    if ((millis() - start_time) > 800) {
      touching = 0;
    }
  }
  pre_dist = dist;
  Serial.println(opened);
}

void light_on() {
  pixels.clear(); 
  for (int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(150, 150, 0));  
  }
  pixels.show();
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
  int x = Wire.read();            // receive byte as an integer
  if (x == 2) {
    opened = 0;
  }
  Serial.println("====close====");
}
// i2c从机发送数据
void requestEvent() {
  // Write short(16bit) number
  buf[0] = byte(opened);
  buf[1] = byte(opened >> 8);
  Wire.write(buf, 2);
}
