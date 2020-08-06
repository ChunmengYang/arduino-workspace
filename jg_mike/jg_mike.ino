#include <Adafruit_NeoPixel.h>
#define MAX_LED 256
#define LED_PIN 9

// 定义常量
const uint8_t kx = 32,
              ky = 8;
const bool layout = false;

// 声音采样频率 ms
const int sampleWindow = 3;
// 声音原始数据
unsigned int sample;
int sensorPin = A0;    // select the input pin for the potentiometer

Adafruit_NeoPixel strip = Adafruit_NeoPixel( MAX_LED, LED_PIN, NEO_RGB + NEO_KHZ800 );
uint32_t color = strip.Color(0,0,255);

int rec_sound() {
  unsigned long startMillis = millis();
  unsigned int peakToPeak = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(0);
    if (sample < 1024)
    {
      if (sample > signalMax) 
      {
        signalMax = sample;
      }
      else if (sample < signalMin)
      {
        signalMin = sample;
      }
    }
  }
  peakToPeak = signalMax - signalMin;
  double volts = (peakToPeak * 3.3) / 1024;
  int sound = (volts * 10);
  // 把声音映射到一个范围 arduino的map函数
  int soundLevel = map(sound, 0, 16, 0, 10);
//  Serial.println(sound);
  return soundLevel;
}

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  strip.begin();
  strip.show();           //显示所有LED为关状态;
  strip.setBrightness(12);
}

void loop() {
  delay(100);
  int level = rec_sound();

  if(level>6){
    led_show(level);
  } else {
    led_show(1);
  }
}

void led_show(int level) {
  int x, y, l;
  
  uint8_t graphic[8][32] = {
    {1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
    {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0}
  };
  
//  for(l=0;l<level*20;l++){
  for (x = 0; x < 32; x++) {
    for (y = 0; y < 8; y++) {
      if (graphic[y][x] == 1) {
        strip.setPixelColor(XY(31-x,7-y), strip.Color(0,0,150));
      }
    }
  }
  if(level==1){
    strip.setBrightness(1);
  }else{
    strip.setBrightness(level*20);
  }
  strip.show();
//  }
//  for(l=level*15;l>=0;l-=3){
//    for (x = n; x < 32; x++) {
//      for (y = 0; y < 8; y++) {
//        if (graphic[y][x] == 1) {
//          strip.setPixelColor(XY(31-x,7-y), strip.Color(0,0,255));
//        }
//      }
//    }
//    strip.setBrightness(l);
//    strip.show();
//  }
}

/**************工具函数***************/

int16_t XY(int8_t x, int8_t y) {
  if (x >= kx || y >= ky  || x < 0 || y < 0) return -1;
  uint16_t n;
  if(x==0){
    n = y;
  }else{
    if(x&1){
      n = x*ky + ky - y - 1;
    }else{
      n = x*ky + y;
    }
  }
  return n;
}

/* 
 */
bool is_on(int n) {
  if (strip.getPixelColor(n) != strip.Color(0, 0, 0)) {
    return true;
  } else {
    return false;
  }
}
/**
 * 灯状态反转
 */
void voxel_reverse(int n, uint32_t color) {
    effect_set_voxel(n,color); 
    strip.show();
}

/**
   某个灯亮
*/
void effect_set_voxel(int n, uint32_t color) {
  strip.setPixelColor(n, color);
}
/**********************工具函数******************/
