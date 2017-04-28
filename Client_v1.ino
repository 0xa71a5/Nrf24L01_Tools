/* Author:At1a5*/
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#define ButtonL 2
#define ButtonR 3
#define ButtonU 5
#define ButtonD 4

enum ButtonType{
  Button_Stop=0,
  Button_Up=1,
  Button_Down=2,
  Button_Left=3,
  Button_Right=4
};


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

void ButtonInit()
{
  pinMode(ButtonL,INPUT_PULLUP);
  pinMode(ButtonR,INPUT_PULLUP);
  pinMode(ButtonU,INPUT_PULLUP);
  pinMode(ButtonD,INPUT_PULLUP);
}
int ButtonPressed(bool continous)
{
  const int buttonAmount=5;
  int buttonIndex[buttonAmount]={-1,ButtonU,ButtonD,ButtonL,ButtonR};
  for(int i=1;i<buttonAmount;i++)
  {
    if(digitalRead(buttonIndex[i])==0)
      {
        if(continous)
        {
          delay(300);
          Serial.println("Stall here");
          Serial.println(i);
          while(digitalRead(buttonIndex[i])==0);
        }
        return i;
      }
  }
  return 0;
}

void setup() {
  Serial.begin(9600);
  Mirf.cePin = 8;
  Mirf.csnPin = 9;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"clie2");
  Mirf.setTADDR((byte *)"serv2");
  Mirf.payload = 32;
  Mirf.config();
  Serial.println("****************Communication Start***********************");
  Serial.println();
  ButtonInit();
}

void loop() {
  int key=ButtonPressed(false);
  switch(key)
  {
    case Button_Stop:
    MirfSend("S");
    break;

    case Button_Left:
    MirfSend("L");
    break;

    case Button_Right:
    MirfSend("R");
    break;

    case Button_Up:
    MirfSend("U");
    break;

    case Button_Down:
    MirfSend("D");
    break;
  }
  delay(50);
}





