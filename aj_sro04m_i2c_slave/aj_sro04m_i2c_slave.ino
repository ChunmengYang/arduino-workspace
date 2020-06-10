#include <Wire.h>
#include <SoftwareSerial.h>
#include <DFMiniMp3.h>

#define SLAVE_ADDRESS 0x05
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
    Serial.println(track);  
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

unsigned long start_time;
short dist;
char buf[2];

void setup ()
{
  softSerial.begin(9600);
  Serial.begin(9600);
  Serial.println("initializing...");
  mp3.begin();
  // 0-30
  mp3.setVolume(24);

  
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
//  Serial.println(mp3.getStatus());
  
  if(dist < 30) {
    if(mp3.getStatus() != 513) {
      mp3.playMp3FolderTrack(1);
    }
    start_time = millis();
  } else {
    if(mp3.getStatus() == 513 && (millis() - start_time) > 600) {
      mp3.stop();
    }
  }
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
  buf[0] = byte(dist);
  buf[1] = byte(dist >> 8);
  Wire.write(buf, 2);
}
