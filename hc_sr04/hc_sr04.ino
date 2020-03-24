#include <NewPing.h>

#define  RIGHT_TRIGGER_PIN 10 //12端口
#define  RIGHT_ECHO_PIN 9 //11端口
#define  LEFT_TRIGGER_PIN 6 //5端口
#define  LEFT_ECHO_PIN 5 //5端口
#define MAX_DISTANCE 50 // Maximum distance (in cm) to ping.

NewPing sonar1 = NewPing(LEFT_TRIGGER_PIN, LEFT_ECHO_PIN, MAX_DISTANCE);
NewPing sonar2 = NewPing(RIGHT_TRIGGER_PIN, RIGHT_ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
}

int d = 50; //采集超声波距离周期
void loop() {
  delay(d);
  
  float d1 = sonar1.ping_cm();
  float d2 = sonar2.ping_cm();
  if (d1 > 1.0) {
     Serial.println(1);
     return;
  }
  if (d2 > 1.0) {
     Serial.println(2);
  }
}
