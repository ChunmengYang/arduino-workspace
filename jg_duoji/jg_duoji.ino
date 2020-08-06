#include <NewPing.h>
#include <Servo.h>

#define  RIGHT_TRIGGER_PIN 10 //D10端口
#define  RIGHT_ECHO_PIN 9 //D9端口
#define  LEFT_TRIGGER_PIN 4 //D4端口
#define  LEFT_ECHO_PIN 3 //D3端口
#define MAX_DISTANCE 20 // Maximum distance (in cm) to ping.
#define JIGUANG_PIN 7 //D7

NewPing sonar1 = NewPing(LEFT_TRIGGER_PIN, LEFT_ECHO_PIN, MAX_DISTANCE);
NewPing sonar2 = NewPing(RIGHT_TRIGGER_PIN, RIGHT_ECHO_PIN, MAX_DISTANCE);
Servo servo;

unsigned long start_time = 0;
float pre_d1 = 0;
float pre_d2 = 0;

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  servo.attach(5);
  pinMode(JIGUANG_PIN, OUTPUT);
}

//int d = 50; //采集超声波距离周期
int jg = 0;
void loop() {
  delay(50);
  if (jg == 10) {
    digitalWrite(JIGUANG_PIN, HIGH);
  } 
  if (jg == 20) {
    digitalWrite(JIGUANG_PIN, LOW);
    jg = 0;
  }
  jg += 1;

  float d1 = sonar1.ping_cm();
  float d2 = sonar2.ping_cm();
  Serial.print("d1的值:");
  Serial.println(d1);
  Serial.print("d2的值:");
  Serial.println(d2);

  bool flag = false;
  // LEFT->RIGHT
  if (pre_d1 > 0 && pre_d2 == 0 && d1 == 0 && d2 > 0) {
    Serial.println(1);
//    servo.writeMicroseconds(2000);
    servo.write(90);
    pre_d1 = 0;
    pre_d2 = 0;
    flag = true;
  }
  // RIGHT->LEFT
  if (pre_d1 == 0 && pre_d2 > 0 && d1 > 0 && d2 == 0) {
    Serial.println(2);
//    servo.writeMicroseconds(1000);
    servo.write(0);
    pre_d1 = 0;
    pre_d2 = 0;
    flag = true;
  }

  if (!flag) {
    if ((d1 > 0 && d2 == 0) || (d1 == 0 && d2 > 0)) {
      pre_d1 = d1;
      pre_d2 = d2;
      start_time = millis();
    }
  }
  if (d1 == 0 && d2 == 0) {
    if ((millis() - start_time) > 1000) {
      pre_d1 = 0;
      pre_d2 = 0;
    }
  }
  

  
//  if (d1 > 1.0) {
//     Serial.println(1);
//     return;
//  }

//  if(d1 != 0 && d2 == 0){
//    Serial.println(3);
//    servo.writeMicroseconds(2000);
//    servo.write(0);
//    delay(1000);
//  }else if(d2 != 0 && d1 ==0 ){
//    Serial.println(4);
//    servo.writeMicroseconds(1000);
//    servo.write(90);
//    delay(1000);
//  }else {
//    
//  }


//  if (d2 >= 5.0 && d2 < 15.0) {
//     Serial.println(1);
//     servo.writeMicroseconds(2000);
//     servo.write(0);
//     delay(1000);
//  }else if(d2 >= 15.0 && d2 < 20.0 ){
//     Serial.println(2);
//     servo.writeMicroseconds(1000);
//     servo.write(90);
//     delay(1000);
//  }
}








//#include <Servo.h>
//#include <NewPing.h>
//
////超声波
//#define  RIGHT_TRIGGER_PIN 10 //D10端口
//#define  RIGHT_ECHO_PIN 9 //D9端口
////#define  LEFT_TRIGGER_PIN 5 //D5端口
////#define  LEFT_ECHO_PIN 4 //D4端口
//#define MAX_DISTANCE 50 // Maximum distance (in cm) to ping.
//Servo servo1;
//int angle = A1;
//
////NewPing sonar1 = NewPing(LEFT_TRIGGER_PIN, LEFT_ECHO_PIN, MAX_DISTANCE);
//NewPing sonar2 = NewPing(RIGHT_TRIGGER_PIN, RIGHT_ECHO_PIN, MAX_DISTANCE);
//
//void setup() {
//  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
//  //舵机
//  pinMode(angle, INPUT);
//  servo1.attach(5);
//  Serial.begin(9600);
//}
//
//int d = 50; //采集超声波距离周期
//void loop() {
////  delay(d);
////  float d1 = sonar1.ping_cm();
//  angle = analogRead(A1);
//  angle = map(angle, 0, 1023, 0, 90);
//  float d2 = sonar2.ping_cm();
//  getDistance(d2,angle);
//  //舵机
//  //Serial.println(angle);
//  Serial.print("angle = ");
//  Serial.println(angle);
//  servo1.write(angle);
//  delay(200);
//////  if (d1 > 1.0) {
//////     Serial.println(1);
//////     return;
//////  }
////  if (d2 > 1.0) {
////     Serial.println(d2);
////  }
//}
////获取距离
//void getDistance(float lengs, int count){
//  if(lengs >= 5 && lengs < 15){
//    Serial.println(lengs);
////    int number = 90;
//    int number = count + 90;
//    servo1.write(number);
//  }else if(lengs >= 15 && lengs <= 20){
//    Serial.println(lengs);
//  }else{
//
//  }
//}
//#include <Servo.h>
//#define DuoPIN  5//定义舵机
//#define Echo 2//定义超声波收
//#define Trig 8//定义超声波发
//#define JiguangPIN  6//定义激光
//Servo myDuoJi;
//int angle = A1;
//long getTime()//超声波发波测距
//{
//  digitalWrite(Trig, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(Trig, LOW);
//  return pulseIn(Echo, HIGH);
//}
//void initChaoShengBo()//初始化超声波端口
//{
//  pinMode(Echo, INPUT); //进口
//  pinMode(Trig, OUTPUT); //出口
//
//}
//void setup()
//{
//  initChaoShengBo();//定义超声波
//  myDuoJi.attach(DuoPIN);//定义舵机
//  pinMode(JiguangPIN, OUTPUT); //出口
//  pinMode(angle, INPUT);
//  Serial.begin(9600);
//}
//void loop()
//{
////  digitalWrite(JiguangPIN,HIGH);
////  delay(1000);
////  digitalWrite(JiguangPIN,LOW);
////  delay(300);
//  angle = analogRead(A1);
//  angle = map(angle, 0, 1023, 0, 90);
//  //Serial.println(angle);
////  Serial.print("angle = ");
////  Serial.println(angle);
//  myDuoJi.write(angle);
//  delay(200);
//  long dis;//一直发波
//  dis = getTime() / 58; //一直测距
//  if (dis >=5 && dis < 15) //当距离小于10cm
//  {
//    myDuoJi.write(90);//SG90舵机转动90度
//    Serial.println(1);
////    delay(2000);//延时两秒
//  }
//  else if(dis >=15 && dis < 20)//除非
//  {
//    myDuoJi.write(90);//转动180度
//    Serial.println(2);
////    delay(2000);//延时两秒
//  }
//}
//#include <Servo.h>
//#define DuoPIN 5//定义舵机
//#define Echo 2//定义超声波收
//#define Trig 8//定义超声波发
//Servo myDuoJi;
//long getTime()//超声波发波测距
//{
//  digitalWrite(Trig, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(Trig, LOW);
//  return pulseIn(Echo, HIGH);
//}
//void initChaoShengBo()//初始化超声波端口
//{
//  pinMode(Echo, INPUT); //进口
//  pinMode(Trig, OUTPUT); //出口
//}
//void setup()
//{
//  initChaoShengBo();//定义超声波
//  myDuoJi.attach(DuoPIN);//定义舵机
//  Serial.begin(9600);//定义公共比特率(应该能省略)
//  
//}
//void loop()
//{
//  long dis;//一直发波
//  dis = getTime() / 58; //一直测距
//  if (dis < 10) //当距离小于10cm
//  {
//    myDuoJi.write(90);//SG90舵机转动90度
//    delay(1000);//延时两秒
//    Serial.println(900);
//  }
//  else//除非
//  {
//    myDuoJi.write(90);//转动180度
//  }
//}

//#include <Servo.h>
//Servo servo1;
//int angle = A1;
//
//void setup()
//{
//  pinMode(angle, INPUT);
//  servo1.attach(5);
//  Serial.begin(9600);
//}
//
//void loop()
//{
//  angle = analogRead(A1);
//  angle = map(angle, 0, 1023, 0, 90);
//  //Serial.println(angle);
//  Serial.print("angle = ");
//  Serial.println(angle);
//  servo1.write(angle);
//  delay(200);
//}
