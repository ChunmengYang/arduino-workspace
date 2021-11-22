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
  aesLib.set_paddingmode(paddingMode::CMS);
}

String encrypt(char * msg, uint16_t msgLen, byte iv[]) {
  int cipherlength = aesLib.get_cipher64_length(msgLen);
  char encrypted[cipherlength];
  Serial.print("encrypt 64 length: "); 
  Serial.println(cipherlength);
  aesLib.encrypt64(msg, msgLen, encrypted, aes_key, sizeof(aes_key), iv);
  return String(encrypted);
}

String decrypt(char * msg, uint16_t msgLen, byte iv[]) {
  char decrypted[msgLen];
  aesLib.decrypt64(msg, msgLen, decrypted, aes_key, sizeof(aes_key), iv);
  return String(decrypted);
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
    String readBuffer = Serial.readStringUntil('\n');
    sprintf(cleartext, "%s", readBuffer.c_str());

    // Encrypt
    uint16_t clen = String(cleartext).length();
    Serial.print("INPUT length:");
    Serial.println(clen);
    Serial.print("INPUT:");
    Serial.println((char*)cleartext);

    memcpy(enc_iv, enc_iv_to, sizeof(enc_iv_to));
    String encrypted = encrypt(cleartext, clen, enc_iv);
    sprintf(ciphertext, "%s", encrypted.c_str());
    Serial.print("Ciphertext: ");
    Serial.println(encrypted);
    delay(1000);


    // Decrypt
    delay(1000);
    uint16_t dlen = encrypted.length();
    memcpy(enc_iv, enc_iv_from, sizeof(enc_iv_from));
    String decrypted = decrypt(ciphertext, dlen, enc_iv);
    Serial.print("Cleartext: ");
    Serial.println(decrypted);
    if (decrypted.equals(cleartext)) {
      Serial.println("SUCCES");
    } else {
      Serial.println("FAILURE");
    }
  }
}
