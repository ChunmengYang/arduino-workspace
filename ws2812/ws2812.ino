
#include "FastLED.h"

#define NUM_LEDS 300             // LED灯珠数量
#define DATA_PIN 6              // Arduino输出控制信号引脚
#define LED_TYPE WS2812          // LED灯带型号
#define COLOR_ORDER GRB          //RGB灯珠中红色、绿色、蓝色LED的排列顺序

uint8_t max_bright = 128;        // LED亮度控制变量，可使用数值为 0 ～ 255，数值越大则光带亮度越高
CRGB leds[NUM_LEDS];             // 建立光带leds

void setup() {
  // put your setup code here, to run once:

  // 设置串口波特率9600
  Serial.begin(9600);
  Serial.println("......setup......");

  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // 初始化光带
  FastLED.setBrightness(max_bright);
}

void loop() {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Red;
      FastLED.show();
    }
  
    delay(1000);

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();

    delay(1000);
}
