
int Pin = 4;//数字4口
int cntValue = 0;
uint8_t prev = 1;
long time = 0;

void setup ()
{
  pinMode(Pin,INPUT);//4号数字口设置为输入状态
  Serial.begin(9600);
  Serial.println(" Count\n");
}
void loop()
{
  int sensorValue = digitalRead(Pin);
//  Serial.println(sensorValue);
  //判断是否被遮挡
  if(sensorValue == 0 && sensorValue != prev) {
    long cur_time = micros();
    long pre_time = time;
    time = cur_time;
    if (pre_time != 0) {
      if((cur_time - pre_time) < 200000) {
        prev = sensorValue;
        return;
      }
      
    }
   
    cntValue++; //计数增加
    Serial.println(cntValue); //串口输出计数值
  }
  prev = sensorValue;
}
