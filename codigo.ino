#include <SPI.h>
#include <Ethernet.h>
#include "MgsModbus.h"

MgsModbus Mb;
//MgsModbus FC;


// Ethernet settings (depending on MAC and Local network)
byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0x94, 0xB5 };
IPAddress ip(192, 168, 0, 100);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

//word Coil;
int contador=0;
int contador1=0;
int contador2=0;
int contador3=0;
int contador4=0;
int contadorproceso=0;
int contadorproceso3=0;
int cont=0;
int contadParo=0;

void setup(){
  Serial.begin(9600);
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  Ethernet.begin(mac, ip, gateway, subnet);  
  
}

void loop(){
  Mb.MbData[0] = 0;
  
  while(true){
  
  digitalWrite(6,bitRead( Mb.GetBit(0x00),0));//start
  digitalWrite(7,bitRead( Mb.GetBit(0x01),0));//stop
  digitalWrite(5,bitRead( Mb.GetBit(0x02),0));//Item at entry
  digitalWrite(8,bitRead( Mb.GetBit(0x03),0));//Item at exit
  digitalWrite(9,bitRead( Mb.GetBit(0x04),0));//Item detected
  
  if(digitalRead(7)==1){
    contadParo=5;
  }
  if(digitalRead(6)==1){
    contadParo=0;
  }
  Serial.println(contadParo);
  if((digitalRead(6)==1||digitalRead(8)==1||contador==1)&&contadParo==0){
    Mb.SetBit(0x06,true);//entry conveyor
    contador=1;
    contadorproceso=0;
  }
  if(digitalRead(5)==1||contador1==1){
    Mb.SetBit(0x07,true);//move Z
    Mb.SetBit(0x06,false);//move Z
    contador=0;
    contador1=1;
  }
  if(digitalRead(9)==1||contador2==1){
    Mb.SetBit(0x09,true);//Grab
    Mb.SetBit(0x07,false);
    Mb.SetBit(0x08,true);
    Mb.SetBit(0x06,false);//move Z
    contador1=0;
    contador2=1;
    contadorproceso=contadorproceso+1;
  }
  if(contadorproceso>=200){
    contador2=0;
    Mb.SetBit(0x09,false);
    Mb.SetBit(0x06,false);
  }
  if(digitalRead(8)==1||contador3==1){
    Mb.SetBit(0x0A,true);
    contador3=1;
  }
  Mb.MbsRun();
  }
}
