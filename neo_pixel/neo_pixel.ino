#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define SWITCH_R_PIN   8
#define SWITCH_L_PIN   9
#define LED_PIN        6
#define DELAYVAL     200

int led_count = 1024;
Adafruit_NeoPixel pixels(led_count, LED_PIN, NEO_GRB + NEO_KHZ800);


void fill(short r, short g, short b) {
  if (led_count == 0) return;
  for(int i=0; i<led_count; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
}

void draw_led_count() {
  display.clearDisplay();

  display.setTextSize(4);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner

  display.print(led_count);
  display.display();
}

void setup() {
  Serial.begin(9600);

  pixels.begin();
  delay(DELAYVAL);
  fill(0, 0, 0);
  led_count = 0;
  delay(DELAYVAL);
    
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  draw_led_count();
}

int read_switch(int pin) {
  return digitalRead(pin);
}

int color_index = 0;
int switch_l_countor = 0;
int switch_r_countor = 0;
void loop() {
  if(read_switch(SWITCH_L_PIN) == 1) {
    fill(0, 0, 0);
    if (switch_l_countor > 10) {
        led_count -= 10;
    } else {
      led_count -= 1;
    }
    if(led_count < 0) {
      led_count = 0; 
    }
    pixels.updateLength(led_count);
    draw_led_count();
    Serial.println(led_count);
    switch_l_countor += 1;
  } else {
    switch_l_countor = 0;  
  }
  if(read_switch(SWITCH_R_PIN) == 1) {
    fill(0, 0, 0);
    if (switch_r_countor > 10) {
      led_count += 10;
    } else {
      led_count += 1;
    }
    if(led_count > 1024) {
      led_count = 1024;
    }
    pixels.updateLength(led_count);
    draw_led_count();
    Serial.println(led_count);
    switch_r_countor += 1;
  } else {
    switch_r_countor = 0;
  }

  if (color_index <= 10) {
    fill(200, 0, 0);
    color_index += 1;
  } else if (color_index > 10 && color_index <= 20) {
    fill(0, 200, 0);
    color_index += 1;
  } else if (color_index > 20 && color_index <= 30) {
    fill(0, 0, 200);
    color_index += 1;
  } else {
    color_index = 0;  
  }
  delay(DELAYVAL);
}
