/* Author:At1a5*/
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
char scanAddress[]="scanF";
char myAddress[6];
char hisAddress[6];
int myChannel=0;
unsigned char dataToRecv[32];
String strs = "";
void broadcastChannel()
{
  Mirf.setTADDR((uint8_t*)scanAddress);
  Mirf.channel=myChannel;
  Mirf.config();
  String strsCombineAddress=myAddress;
  strsCombineAddress+=":Broadcast for receiver";
  unsigned long lastTime=millis();
  while(millis()-lastTime<2000)
  MirfSend(strsCombineAddress);
}
int  scan()
{
Serial.println("Scanning...");
Mirf.setRADDR((uint8_t*)scanAddress);
bool findChannel=false;
int channelFound=-1;
for(int i=0;i<125;i++)
{
 // Serial.println(i);
  Mirf.channel=i;
  Mirf.config();
  unsigned long lastTime=millis();
  
  while(millis()-lastTime<20)
  {
    if(Mirf.dataReady())
    {
      findChannel=true;
      channelFound=i;
      break;
    }
  } 
  if(findChannel)
  {
    
    Mirf.getData(dataToRecv);
    Mirf.channel=myChannel;
    Mirf.config();
    Mirf.setRADDR((uint8_t*)myAddress);
     Serial.println("Found channel active");
     Serial.print("Channel:");
     Serial.println((unsigned int)channelFound);
     Serial.println((char *)dataToRecv);
    return channelFound;
  }
}

Mirf.channel=myChannel;
Mirf.config();
Mirf.setRADDR((uint8_t*)myAddress);
return -1;
}
void MirfSend(char dataSend[])
{
  Mirf.send((unsigned char *)dataSend);
  while (Mirf.isSending());
}
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
void setMyAddress()
{
 // char address[6];
  int index = 0;
  Serial.println("Please enter your address(5 characters like '12345')");
  while (Serial.available() == 0);
  while (Serial.available() != 0)
  {
    char data = Serial.read(); delay(2);
    myAddress[index++] = data;
  }
  myAddress[5]='\0';
  Serial.print("Your address is:");
  Serial.println(myAddress);
  Mirf.setRADDR((unsigned char *)myAddress);

}
void setHisAddress()
{
  
  int index = 0;
  Serial.println("Please enter his address(5 characters like '12345')");
  while (Serial.available() == 0);
  while (Serial.available() != 0)
  {
    char data = Serial.read(); delay(2);
    hisAddress[index++] = data;
  }
  hisAddress[5]='\0';
  Serial.print("His address is:");
  Serial.println(hisAddress);
  
  Mirf.setTADDR((unsigned char *)hisAddress);
}
void setChannel()
{
  int channel=0;
   Serial.println("Set channel(0-124):");
  while (Serial.available() == 0);
  while (Serial.available() != 0)
  {
    char data = Serial.read(); delay(2);
   channel*=10;
   channel+=data-'0';
  }
  Serial.print("Your channel is:");
  Serial.println(channel);
  Mirf.channel=channel;
  Mirf.config();
  myChannel=channel;
}
void setup() {
  Serial.begin(9600);
  Mirf.cePin = 8;
  Mirf.csnPin = 9;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"00000");
  Mirf.setTADDR((byte *)"00001");
  Mirf.payload = 32;
  Mirf.config();
  
  setMyAddress();
  setHisAddress();
  setChannel();
  Serial.println();
  Serial.println("Enter \\c to change your channel;\nEnter \\m to change your address;\nEnter \\h to change his address;");
  Serial.println("Enter \\s to scan active channel;");
  Serial.println("Enter \\b to scan active channel;");
  Serial.println("****************Communication Start***********************");
  Serial.println();
}

void loop() {
  while (Serial.available())
  {
    char data=Serial.read();
    strs+=data;
    delay(2);
    if(data=='\\')
    {
      char dataFlag=Serial.read();
      delay(2);
      switch(dataFlag)
      {
        case 'm':
        setMyAddress();
        strs="";
        break;
        
        case 'h':
        setHisAddress();
        strs="";
        break;
        
        case 'c':
       setChannel();
        strs="";
        break; 

        case 's':
        while(Serial.available()==0)
  {
   Serial.println("Scanning");
   uint8_t result=scan();
   if(result!=-1)
   {

     break;
   }
  }
        strs="";
        break; 

        case 'b':
        while(Serial.available()==0)
        {
          Serial.println("Broad...");
        broadcastChannel();
        }
        strs="";
        break; 
        
      }
    }
    if(strs=="")
    break;
  }
  if (strs.length() != 0)
  {
    
    Serial.print("I:");
    Serial.print(strs);
    Serial.print("    [CH:");
    Serial.print(myChannel);
    Serial.println("]");
    String strsCombineAddress=myAddress;
    strsCombineAddress+=":";
    strsCombineAddress+=strs;
    MirfSend(strsCombineAddress);
    strs = "";
  }

  if (Mirf.dataReady()) {

    Mirf.getData(dataToRecv);
    Serial.print((char*)dataToRecv);
    Serial.print("    [CH:");
    Serial.print(myChannel);
    Serial.println("]");
  }
}



