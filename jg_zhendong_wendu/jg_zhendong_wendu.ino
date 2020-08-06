#include <Wire.h>   //IIC头文件
#include <Adafruit_NeoPixel.h>
#define MAX_LED 32
#define LED_PIN 6

// constants won't change. They're used here to set pin numbers:
// the number of the pushbutton pin
//定义振动传感器针脚
const int shockPin=7;
//定义温度传感器模拟针脚
//const int airPin=A0;
 // the number of the LED pin
Adafruit_NeoPixel strip = Adafruit_NeoPixel( MAX_LED, LED_PIN, NEO_RGB + NEO_KHZ800 );
uint32_t color = strip.Color(0,0,255);
uint32_t color1 = strip.Color(0,0,0);
// variables will change:
// variable for reading the pushbutton status
//定义震动传感器常量
int shocknumber=0;
//定义温度传感器常量
//int airnumber=0;



void setup() {
  Wire.begin();   // join i2c bus (address optional for master)
  // initialize the LED pin as an output:
  pinMode(LED_PIN, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(shockPin,INPUT); 
  // pinMode(airPin,INPUT); 
  strip.begin();
  strip.show();           //显示所有LED为关状态;
  strip.setBrightness(30);
  //打开串口监视器
  Serial.begin(9600);
}

uint16_t result;
float value;

void loop() {
  //获取震动传感器状态
  shocknumber=digitalRead(shockPin);
  //判断是否亮or灭
  if(shocknumber==LOW){
    Wire.beginTransmission(0x5A);
    Wire.write(0x07);                // sends instruction byte
    Wire.endTransmission(false);     // stop transmitting
   
    Wire.requestFrom(0x5A, 3);   //Send data n-bytes read
    result = Wire.read();        //Receive DATA
    result |= Wire.read() << 8;  //Receive DATA
   
    uint8_t pec = Wire.read();
    
    value =  result*0.02-273.15;  //温度数值转换
    Serial.println(value);

    int count = int((value - 30) * 5);
    if (count < 0) {
        count = 0;
    }
    if (count > 36) {
        count = 36;
    } else {
        strip.clear();
        for (int i = 0; i < count; i++) {
          strip.setPixelColor(i, color);
        }
        strip.show();
    }
  } else {
    led_make();
  } 
}



/***************震动********************/
void led_make(){
  for(int p=0;p<5;p++){
    all_on();
    delay(30);
    all_off();
    delay(30);
  }
}
void all_on(){
  Serial.println(100);
  strip.fill(color);
  strip.show();
}
void all_off(){
  Serial.println(200);
  strip.clear();
  strip.show();
}










/**************温度******************/
//void led_all_on(){
//    all_on(32,color);
//    delay(10);
//    all_on_back(32,color1);
//    delay(10);
//}
//void all_on(int led_count,uint32_t color){
//  
//  for(int i=0;i<led_count;i++){
//      Serial.println(8600);
//      strip.setPixelColor(i,color);
//      strip.show();
//      delay(10);
//  }
//  
//}
//void all_on_back(int led_count,uint32_t color){
//  
//  for(int k=led_count-1; k>=0; k--){
//    strip.setPixelColor(k,color);
//    strip.show();
//    delay(10);
//  }
//}
