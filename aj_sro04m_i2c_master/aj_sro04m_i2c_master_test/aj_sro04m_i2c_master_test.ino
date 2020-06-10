unsigned long startTime;
unsigned long interval = 5000;
String result = "0,0,0,0,0";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  randomSeed(analogRead(0));
}


void loop() {
  unsigned long endTime = millis();
  if((endTime - startTime) > interval) {
      startTime = endTime;
      unsigned long values[5] = {0, 0, 0, 0, 0};
      long num = random(0, 10);
      if (num > 0) {
          for (short i = 0; i < 5; i++) {
            long temp = random(0, 6);
            if (temp > 2) {
                values[i] = 20;
            }
          }
          interval = 5000; 
      } else {
          interval = 16000;  
      }
      
      result = "";
      for (short i = 0; i < 5; i++) {
        char s[2];
        itoa(values[i], s, 10);
        
        result = result + s;
        if (i < 4) {
           result = result + ',';
         }
      }
  }
  
  Serial.println(result);
  delay(200);
}
