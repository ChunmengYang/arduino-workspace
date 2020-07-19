#include <Wire.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define SLAVE_ADDRESS 0x04
#define  TRIGGER_PIN 9
#define  ECHO_PIN 10

SoftwareSerial softSerial(8, 7); // RX, TX
DFRobotDFPlayerMini mp3;

short mp3FolderTrack = 1;

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
  
  softSerial.begin(9600);
  mp3.begin(softSerial);
  // 0-30
  mp3.volume(30);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // join i2c bus with address SLAVE_ADDRESS
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("starting...");
}

void loop()
{
  delay(100);
  dist = readcm(TRIGGER_PIN, ECHO_PIN);
//  Serial.println(dist);

  if (dist < 30 && pre_dist < 30) {
    if (touching == 0) {
      if (opened == 0) {
        opened = 1;
        if (mp3.readState() != 513) {
          mp3FolderTrack += 1;
          if (mp3FolderTrack > 4) {
            mp3FolderTrack = 1;
          }
          Serial.println(mp3FolderTrack);
          mp3.loop(mp3FolderTrack);
        }
      } else {
        opened = 0;
        if (mp3.readState() != 513) {
          mp3.stop();
        }
      }
      touching = 1;
    }

    start_time = millis();
  } else {
    if ((millis() - start_time) > 1000) {
      touching = 0;
    }
  }
  pre_dist = dist;
//  Serial.println(opened);
}

float readcm(int trigger_pin, int echo_pin) {
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);

  dist = pulseIn(echo_pin, HIGH) / 58.00;
  return dist;
}

void requestEvent() {
  // Write short(16bit) number
  buf[0] = byte(opened);
  buf[1] = byte(opened >> 8);
  Wire.write(buf, 2);
}
