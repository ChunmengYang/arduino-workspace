#define  ECHO_PIN 5

void setup ()
{
  Serial.begin(9600);
  pinMode(ECHO_PIN, INPUT);
  Serial.println("setup\n");
}

void loop()
{
  delay(200);
  int pirValue = digitalRead(ECHO_PIN);
  Serial.println(pirValue);
}
