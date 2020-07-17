#include <Wire.h>
#include <SoftwareSerial.h>
#include <DFMiniMp3.h>

#define SLAVE_ADDRESS 0x03
#define  TRIGGER_PIN 9
#define  ECHO_PIN 10

// implement a notification class,
// its member methods will get called
class Mp3Notify
{
  public:
    static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
    {
      if (source & DfMp3_PlaySources_Sd)
      {
        Serial.print("SD Card, ");
      }
      if (source & DfMp3_PlaySources_Usb)
      {
        Serial.print("USB Disk, ");
      }
      if (source & DfMp3_PlaySources_Flash)
      {
        Serial.print("Flash, ");
      }
      Serial.println(action);
    }
    static void OnError(uint16_t errorCode)
    {
      // see DfMp3_Error for code meaning
      Serial.println();
      Serial.print("Com Error ");
      Serial.println(errorCode);
    }
    static void OnPlayFinished(DfMp3_PlaySources source, uint16_t track)
    {
      Serial.print("Play finished for #");
      Serial.println();
      
    }
    static void OnPlaySourceOnline(DfMp3_PlaySources source)
    {
      PrintlnSourceAction(source, "online");
    }
    static void OnPlaySourceInserted(DfMp3_PlaySources source)
    {
      PrintlnSourceAction(source, "inserted");
    }
    static void OnPlaySourceRemoved(DfMp3_PlaySources source)
    {
      PrintlnSourceAction(source, "removed");
    }
};

// instance a DFMiniMp3 object,
// defined with the above notification class and the hardware serial class
//
SoftwareSerial softSerial(8, 7); // RX, TX
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(softSerial);

short mp3FolderTrack = 1;

unsigned long start_time;
short pre_dist = 0;
short dist;
short touching = 0;
short opened = 0;
char buf[2];


void setup ()
{
  softSerial.begin(9600);
  Serial.begin(9600);
  Serial.println("initializing...");
  mp3.begin();
  mp3.reset();
  // 0-30
  mp3.setVolume(28);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // join i2c bus with address SLAVE_ADDRESS
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("starting...");
}

void loop()
{
  delay(200);
  dist = readcm(TRIGGER_PIN, ECHO_PIN);
  Serial.println(dist);

  if (dist < 30 && pre_dist < 30) {
    if (touching == 0) {
      if (opened == 0) {
        opened = 1;
        if (mp3.getStatus() != 513) {
          mp3FolderTrack += 1;
          if (mp3FolderTrack > 4) {
            mp3FolderTrack = 1;
          }
          mp3.playMp3FolderTrack(mp3FolderTrack);
        }
      } else {
        opened = 0;
        if (mp3.getStatus() == 513) {
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
  Serial.println(opened);
  if (opened == 1) {
    if (mp3.getStatus() != 513) {
      mp3.playMp3FolderTrack(mp3FolderTrack);
    }
  }
  pre_dist = dist;
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
