#include "AESLib.h"

AESLib aesLib;

char cleartext[128];
char ciphertext[256];

// AES Encryption Key
byte aes_key[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66};

// General initialization vector (you must use your own IV's in production for full security!!!)
byte aes_iv[N_BLOCK] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};

// Generate IV (once)
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


void setup() {
  Serial.begin(9600);
  while (!Serial); // wait for serial port
  delay(2000);
  aes_init();

  Serial.println("Enter text to be encrypted into console (no feedback) and press ENTER (newline):");
}

byte enc_iv[N_BLOCK] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};
byte enc_iv_to[N_BLOCK] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};
byte enc_iv_from[N_BLOCK] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};

void loop() {
  if (Serial.available()) {
    String en_buffer = Serial.readStringUntil('\n');

    uint16_t en_len = en_buffer.length();
    char en_byte[en_len];
    sprintf(en_byte, "%s", en_buffer.c_str());
    Serial.print("INPUT length:");
    Serial.println(en_len);
    Serial.print("INPUT:");
    Serial.println((char*)en_byte);
    Serial.println("--------------------------");

    // Encrypt
    memcpy(enc_iv, enc_iv_to, sizeof(enc_iv_to));
    uint16_t encLen = encrypt((byte *)en_byte, en_len, enc_iv);
    Serial.print("Encrypted length: ");
    Serial.println(encLen);
    Serial.print("Encrypted result: ");
    Serial.println((char*)ciphertext);
    Serial.println("--------------------------");

    
    // Decrypt
    String de_buffer = String(ciphertext);
    char de_byte[de_buffer.length()];
    sprintf(de_byte, "%s", de_buffer.c_str());


    char b64dec[base64_dec_len(de_byte, sizeof(de_byte))];
    base64_decode(b64dec, de_byte, sizeof(de_byte));
    
    uint16_t de_len = sizeof(b64dec);
    memcpy(enc_iv, enc_iv_from, sizeof(enc_iv_from));
    uint16_t decLen = decrypt((byte *)b64dec, de_len, enc_iv);
    Serial.print("Decrypted length: "); 
    Serial.println(decLen);
    Serial.print("Decrypted result: ");
    Serial.println(String(cleartext));
    
  
    if (strcmp((char*)en_byte, (char*)cleartext) == 0) {
      Serial.println("Decrypted correctly.");
    } else {
      Serial.println("Decryption test failed.");
    }
    Serial.println("==========================");
  }
}
