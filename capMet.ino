

/*
 TDC7200 time to Voltage Converter in Capacitance Measurement

 Circuit:
 TDC7200 --> Arduino Uno 
 1 --> D2
 2 --> D3
 3(start) --> D4
 4(stop) --> 
 8 --> D9
 9 --> D12
 10 --> D11
 11 --> D8
 12 --> D13

 created 30 January 2017 
 by Mohan Gopal Tripathi
 */


#include "SPI.h"
#include <Arduino.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);


const byte READ = 0x00;
const byte WRITE = 0x40;
float CLKPRD = 0.0625E-6;
int ENABLE = 2;
int TRIGG = 3;
int START = 4;
int STOP = 5;
int INTB = 10;
int CSB = 9;


void setup() {

 Serial.begin(9600);
 u8g2.begin();

 pinMode(ENABLE, OUTPUT);
 pinMode(START, OUTPUT);
 pinMode(STOP,OUTPUT);
 pinMode(CSB,OUTPUT);
 pinMode(TRIGG,INPUT);
 pinMode(INTB,INPUT);
 pinMode(5,OUTPUT);
 pinMode(7,INPUT);




}
 


 


void loop() {

  unsigned long TM1=0,TM2=0,CALONE=0,CALTWO=0,CLKCNT1=0,CONFIG=0;
  float CALCNT=0,normLSB=0,TOF=0,capacitance=0;

  

 //digitalWrite(5,LOW);
 //delay(100);
/* Step1: low to high transition on ENABLE*/
                                                                                                                                                                                                                                                                                                                                                                                                                 
 digitalWrite(ENABLE,LOW);
 //delay(1);
 digitalWrite(ENABLE,HIGH);
 delay(2);   //waiting time to start up a tdc 7200 after enable

/* Step2 : Initiating SPI*/

 SPI.begin();
 //Serial.println(readRegister(0x00,1));
 //Serial.println(readRegister(0x10,3));


 /*writeRegister(0x10,0x000);//TIME1
 writeRegister(0x12,0x000);//TIME2
 writeRegister(0x1B,0x000);//CALIBRATION1
 writeRegister(0x1C,0x000);//CALIBRATION2
 writeRegister(0x11,0x000);//CLOCK_COUNT1*/




 /* Step3 : Configuring registers of TDC7200*/ 
 writeRegister(0x00,0x03);//CONFIG1 Register for Measurement Mode 1 = 01H or Measurement Mode 2 = 03H
 writeRegister(0x01,0x00);  //CONFIG2 Register 0x00H

 
 
 
  
  /* ---------- giving START and STOP pulse for measurement ----------*/
  
digitalWrite(START,HIGH);
digitalWrite(5,HIGH);
  
  
Serial.println(readRegister(0x00,1));
//Serial.println(readRegister(0x00,1));
  
TM1 = readRegister(0x10,3);
  //Serial.print( "TIMER1 :");
  //Serial.println(readRegister(0x10,3));

TM2 = readRegister(0x12,3);
  //Serial.print( "TIMER2 :");
 // Serial.println(readRegister(0x12,3));

  
  //Serial.print( "INTSTATUS :");
  //Serial.println(readRegister(0x02,1));
  
CALONE = readRegister(0x1B,3);
  //Serial.print( "CALIBRATION1 :");
  //Serial.println(readRegister(0x1B,3));
  
CALTWO = readRegister(0x1C,3);
  //Serial.print( "CALIBRATION2 :");
  //Serial.println(readRegister(0x1C,3));

//CLKCNTONE = readRegister(0x11,3);
  //Serial.print( "Clock Count 1 :");
  //Serial.println(readRegister(0x11,3));
  
  
 CLKCNT1 = readRegister(0x11,3);
  
 


CALCNT = (CALTWO - CALONE)/2;
  //Serial.print( "calCount :");
  //Serial.println(CALCNT);
  
  
normLSB = 0.0625E-6/CALCNT;
  //Serial.print( "normLSB :");
  //Serial.println(normLSB,2);
  
  
TOF = ((normLSB*TM1)-(normLSB*TM2))+(CLKCNT1)*(CLKPRD);
  //Serial.print("Final Time : ");
  //Serial.println(TOF,4);


capacitance = TOF/0.6020;
capacitance = capacitance*1000000;
  //Serial.println();
//Serial.print("Capacitance : ");
Serial.println(capacitance,4);



   enum {BufSize=9}; // If a is short use a smaller number, eg 5 or 6 
   char buf[BufSize];

   double x = capacitance;
   double fractpart, intpart;
   fractpart = modf(x, &fractpart);
   int fract = (int)(fractpart*100);
   int intparty = (int)x;
   snprintf (buf, BufSize, "%d.%d", intparty, fract);
 
   u8g2.clearBuffer();         // clear the internal menory
   u8g2.setFont(u8g2_font_ncenB14_tr); // choose a suitable font
   u8g2.drawStr(80,40,"pF");
   u8g2.drawStr(10,40,buf);  // write something to the internal memory
   u8g2.sendBuffer();
   //delay(5);


   //Serial.println(readRegister(0x00,1));


   


  //Serial.println();
  //Serial.println("pF");
  //pinMode(10,LOW);
  //pinMode(10,INPUT);
  //digitalWrite(5,LOW);
  digitalWrite(5,LOW);
  delay(100);
  //delay(100);
  
  digitalWrite(7,LOW);
  pinMode(7,OUTPUT);
}









unsigned long readRegister(byte addrr, int readsize)
{
  byte value = 0x00;
  unsigned long result = 0;

  byte data = addrr | READ;
  digitalWrite(CSB, LOW);
  SPI.transfer(data);


  for(int i=0;i<readsize;i++)
  {
    value = SPI.transfer(0x00);
    result = result << 8;
    result = result | value;
  }
  digitalWrite(CSB, HIGH);
  return (result);
}

void writeRegister(byte addrr, byte theValue)
{
  byte data = addrr | WRITE;
  digitalWrite(CSB, LOW);
  SPI.transfer(data);
  SPI.transfer(theValue);
  digitalWrite(CSB, HIGH);
}




  
  
  
  
  
  
  
  
 
