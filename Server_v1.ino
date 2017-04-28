/* Author:At1a5*/
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 7
#define ENL 6 //PWM left
#define ENR 5 //PWM right

#define offsetL 0
#define offsetR 0

#define leftForward() digitalWrite(IN1,1);digitalWrite(IN2,0);
#define leftBackward() digitalWrite(IN1,0);digitalWrite(IN2,1);
#define rightForward() digitalWrite(IN3,1);digitalWrite(IN4,0);
#define rightBackward() digitalWrite(IN3,0);digitalWrite(IN4,1);

unsigned char dataToRecv[32];
String strs = "";

void MirfSend(String data)
{
  char dataSend[30];
  int i;
  for (i = 0; i < data.length(); i++)
    dataSend[i] = data[i];
  dataSend[i] = '\0';
  Mirf.send((unsigned char *)dataSend);
  while (Mirf.isSending());
}

String MirfReadAll()
{
  unsigned char data[32];
  String retStr;
  if(Mirf.dataReady()) {
    Mirf.getData(data);
    retStr=String((const char *)data);
  }
  return retStr;
}

void MotorInit()
{
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  digitalWrite(IN1,0);
  digitalWrite(IN2,0);
  digitalWrite(IN3,0);
  digitalWrite(IN4,0);
  analogWrite(ENL,0);
  analogWrite(ENR,0);
}
#define defaultSpeed 30
void forward(int num=defaultSpeed)
{
  if(num>100)num=100;
  if(num<0)num=0;
  num=int(float(num)*2.55);
  leftForward();
  rightForward();
  analogWrite(ENL,num);
  analogWrite(ENR,num);
}
void backward(int num=defaultSpeed)
{
  if(num>100)num=100;
  if(num<0)num=0;
  num=int(float(num)*2.55);
  leftBackward();
  rightBackward();
  analogWrite(ENL,num);
  analogWrite(ENR,num);
}
void turnleft(int num=defaultSpeed)
{
  if(num>100)num=100;
  if(num<0)num=0;
  num=int(float(num)*2.55);
  leftBackward();
  rightForward();
  analogWrite(ENL,num);
  analogWrite(ENR,num);
}
void turnright(int num=defaultSpeed)
{
  if(num>100)num=100;
  if(num<0)num=0;
  num=int(float(num)*2.55);
  leftForward();
  rightBackward();
  analogWrite(ENL,num);
  analogWrite(ENR,num);
}
void stopcar()
{
  forward(0);
}

void setup() {
  Serial.begin(9600);
  Mirf.cePin = 8;
  Mirf.csnPin = 9;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"serv2");
  Mirf.setTADDR((byte *)"clie2");
  Mirf.payload = 32;
  Mirf.config();
  Serial.println("****************Communication Start***********************");
  Serial.println();
  MotorInit();
  stopcar();
}

void () {
  if(Mirf.dataReady())
  {
    String recvCommand=MirfReadAll();
    if(recvCommand.length()>=1)
    {
      char flag=recvCommand[0];
      switch(flag)
      {
        case 'L':
        Serial.println("left");
        turnleft();
        break;

        case 'R':
        Serial.println("right");
        turnright();
        break;

        case 'U':
        Serial.println("up");
        forward();
        break;

        case 'D':
        Serial.println("down");
        backward();
        break;

        case 'S':
        Serial.println("S");
        stopcar();
        break;
      }
      delay(20);
    }
  }
}





