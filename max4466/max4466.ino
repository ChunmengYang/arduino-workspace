#include <Wire.h>

#define SLAVE_ADDRESS 0x06
#define  MIC_IN A0

// 声音采样频率 ms
const int sampleWindow = 200;
// 声音原始数据
unsigned int sample;
//声音等级
short soundLevel;


void setup ()
{
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDRESS); // join i2c bus with address 0x04
  Wire.onRequest(requestEvent);
  Serial.println("setup\n");
}

void loop()
{
  double soundSensed = sampleSoundPeak();
  
  // convert to volts
  double volts = (soundSensed * 3.3) / 1024; 

  // 把声音映射到一个范围 arduino的map函数
  soundLevel = map(volts, 0, 3.3, 0, 255);
  
  // print it out
  Serial.println(soundLevel);
}

/**
 * Sense biggest input difference are being input from the analog MIC sensor
 * over a certain "window" of time. 
 * Values returned are in the range 0 - 1024.
 **/
double sampleSoundPeak()
{
    // record start time 
    double startMillis = millis(); 

    // this will be the highest peak, so start it very small    
    int signalMax = 0;
    
    // this will be the lowest peak, so start it very high
    int signalMin = 1024;
    
    // will hold the current value from the microphone
    int sample;
    
    // collect data for 50 ms
    while ((millis() - startMillis) < sampleWindow) 
    {
        // read a value from mic and record it into sample variable
        sample = analogRead(MIC_IN);
        
        // toss out spurious readings
        if (sample < 1024)
        {
        
            // if the current sample is larger than the max
             if (sample > signalMax)
             {      
                   // this is the new max -- save it
                   signalMax = sample; 
             }
             // otherwise, if the current sample is smaller than the min
             else if (sample < signalMin)
             {
                   // this is the new min -- save it
                   signalMin = sample; 
             }
         }
     }
     
     // now that we've collected our data,
     // determine the peak-peak amplitude as max - min
     int peakDifference = signalMax - signalMin; 
    
     // give it back to the caller of this method
     return peakDifference;
}

void requestEvent() {
  short sound = soundLevel;
  Wire.write(sound);
}
