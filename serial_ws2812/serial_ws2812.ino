
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
}

int index = 0;
int ledCount = 1024;

void loop() {
  uint16_t dataLength = 18 + ledCount * 3;

  uint8_t ledData[dataLength] = {0};
  //头部赋值
  ledData[0] = 0xDD;
  ledData[1] = 0x55;
  ledData[2] = 0xEE;
  //组地址赋值
  uint16_t groupAddress = 0;
  ledData[3] = (uint8_t) (groupAddress >> 8);
  ledData[4] = (uint8_t) groupAddress;
  //设备地址赋值
  uint16_t deviceAddress = 1;
  ledData[5] = (uint8_t) (deviceAddress >> 8);
  ledData[6] = (uint8_t) deviceAddress;
  //端口号赋值
  uint8_t port = 0;
  ledData[7] = port;
  //功能码赋值
  ledData[8] = 0x99;
  //灯带类型赋值
  ledData[9] = 0x01;
  //保留的两个字节赋值
  ledData[10] = 0x00;
  ledData[11] = 0x00;
  //数据长度赋值
  uint16_t rgbLenth = ledCount * 3;
  ledData[12] = (uint8_t) (rgbLenth >> 8);
  ledData[13] = (uint8_t) rgbLenth;
  //扩展次数赋值
  uint16_t repeat = 1;
  ledData[14] = (uint8_t) (repeat >> 8);
  ledData[15] = (uint8_t) repeat;
  //尾部字节赋值
  ledData[dataLength - 2] = 0xAA;
  ledData[dataLength - 1] = 0xBB;

  if (index == 0) {
    for(int i = 0; i < ledCount; i++) {
      int ledIndex = 16 + i*3;
      ledData[ledIndex] = 0xFF;
      ledData[ledIndex + 1] = 0x00;
      ledData[ledIndex + 2] = 0x00;
    }
    
    index = 1;
  } else if (index == 1) {
    for(int i = 0; i < ledCount; i++) {
      int ledIndex = 16 + i*3;
      ledData[ledIndex] = 0x00;
      ledData[ledIndex + 1] = 0xFF;
      ledData[ledIndex + 2] = 0x00;
    }

    index = 2;
  } else {
    for(int i = 0; i < ledCount; i++) {
      int ledIndex = 16 + i*3;
      ledData[ledIndex] = 0x00;
      ledData[ledIndex + 1] = 0x00;
      ledData[ledIndex + 2] = 0xFF;
    }

    index = 0;
  }

  
  Serial.write(&ledData[0], dataLength);
  delay(50);
}
