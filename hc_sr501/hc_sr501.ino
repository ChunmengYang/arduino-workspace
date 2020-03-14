#define  ECHO_PIN 9

int pirValue = 0;
int counter = 0;

void setup ()
{
  Serial.begin(9600);
  pinMode(ECHO_PIN, INPUT);
  Serial.println("setup\n");
}

void loop()
{
  delay(200);
  pirValue = digitalRead(ECHO_PIN);
  Serial.println(pirValue);
  if (pirValue == LOW) {
    counter = 0;
  } else {
    counter += 1;
  }
  if (counter > 5) {
    counter = 0;
    Serial.println(">>>>>>>>>>");
  }
}
