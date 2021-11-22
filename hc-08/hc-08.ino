#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include "AESLib.h"

SoftwareSerial hc_08(5, 6); // RX-5, TX-6


AESLib aesLib;

char cleartext[128];
char ciphertext[256];

// AES Encryption Key
byte aes_key[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66};

byte aes_iv[N_BLOCK] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};
byte dec_iv[N_BLOCK] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};
byte dec_iv_from[N_BLOCK] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};

void aes_init() {
  aesLib.gen_iv(aes_iv);
  aesLib.set_paddingmode((paddingMode)0);
}

uint16_t encrypt(byte msg[], uint16_t msgLen, byte iv[]) {
  uint16_t enc_length = aesLib.encrypt(msg, msgLen, (char*)ciphertext, aes_key, sizeof(aes_key), iv);
  return enc_length;
}

uint16_t decrypt(byte msg[], uint16_t msgLen, byte iv[]) {
  uint16_t dec_length = aesLib.decrypt(msg, msgLen, (char*)cleartext, aes_key, sizeof(aes_key), iv);
  return dec_length;
}


#define PIN        3
#define NUMPIXELS 16
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  delay(2000);
  aes_init();

  pixels.begin();

  Serial.begin(9600);
  hc_08.begin(9600);

  delay(500);
  hc_08.print("AT+NAME=Forklift");
  delay(500);
  hc_08.print("AT+ADDR=3D2910594F5A");
  delay(500);

  Serial.println("=======setup=======");
}

void loop() {
  hc_08.listen();
  if (hc_08.available()) {
    String buffer = hc_08.readStringUntil('\n');
    uint16_t buf_len = buffer.length();
    Serial.println(buffer);
    Serial.print("buffer length: "); 
    Serial.println(buf_len);

    //String -> char[] -> base64 char[]
    char de_byte[buf_len];
    sprintf(de_byte, "%s", buffer.c_str());
    char dec_byte_b64[base64_dec_len(de_byte, sizeof(de_byte))];
    base64_decode(dec_byte_b64, de_byte, sizeof(de_byte));

    memcpy(dec_iv, dec_iv_from, sizeof(dec_iv_from)); 
    uint16_t dec_len = decrypt((byte *)dec_byte_b64, sizeof(dec_byte_b64), dec_iv);
    String reuslt = String(cleartext);
    Serial.print("Decrypted length: "); 
    Serial.println(dec_len);
    Serial.print("Decrypted result: ");
    Serial.println(reuslt);

    if (reuslt.indexOf("open") >= 0) {
      Serial.println("=======Open LED======");
      // Set LED
      for(int i = 0; i < NUMPIXELS; i++) { // For each pixel...
        pixels.setPixelColor(i, pixels.Color(150, 150, 0));
      }
      pixels.show();

      hc_08.print("OK");
    } else {
      Serial.println("=======Close LED======");
      // Set LED
      pixels.clear();
      pixels.show();
      hc_08.print("Failed");
    }
  }
  delay(100);
}
