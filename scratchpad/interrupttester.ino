//Timer interrupt Test trial
//Jeididah Oyeyemi
//june 25 2013
#include <SPI.h>
#include <RADIO.h>
 
 
//byte RX_Buffer[61]={0};
//byte size,i;
//void RegConfigPrint();
//byte RegBuffer[50]={0};
//byte TX_Buffer[]={11,12,13,14};
//byte i;
int sensorValue = 0;  // variable to store the value coming from the sensor
char mode;//
int  Speed=7812;
unsigned long  count ;
unsigned long start;
int light;


void setup (void)
{
mode=0;
  Serial.begin (9600);
   Serial.flush();
  Serial.print("set up");
   SPI.begin ();
   SPI.setClockDivider(SPI_CLOCK_DIV4);   
   SPI.setBitOrder(MSBFIRST);   
  SPI.setDataMode(SPI_MODE0) ;
  light=0;
  pinMode(6,OUTPUT);

Serial.println(Speed);

}
void loop(void){
 Serial.flush();
 Serial.println("frame start");
 setInterrupt1(); 
 delay(1000);
  Serial.println("frame end");
}
void setInterrupt1(void)//sets up the conditions of interrupt1
 {  noInterrupts();           // disable all interrupts
   // initialize timer1 
//  Serial.print("set");
  
 //setting up SPI 
 
 //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = Speed;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  interrupts();             // enable all interrupts
  
      }
void setInterrupt2(void)//sets up the conditions for interrupt 2
{
      }
ISR(TIMER1_COMPA_vect)//first timer interrupt
{Serial.flush();
  Serial.println("work?");
 // noInterrupts();
//  start=millis();
  for(int i=0; i<10;i++)
  {int x=7;
   Serial.print(i);
  }
  delayMicroseconds(2000000);
  unsigned long now;
  Serial.println(start);
  count=0;
  int counti=0;
if(mode==0)
{ Speed=470;
  toggle();
  Serial.println("mode0");
  mode=1;
 
}
else{
  Speed=310;
  toggle();

 unsigned long interval=300;
 unsigned long limit=200000;
  Serial.println("mode1");
 /*while(counti<300)// this is the while loop that is intended to  make a delay
  {
    count=(millis()-start);
   Serial.println(counti);
  //  counti=(int)count;
   counti++;
   interval++;
   // toggle();
  }*/
  delay(5000);
 mode=0;
 Serial.println("count:");
 Serial.println(count);
}
//count=(millis()-start);
now=millis();
counti=(int) count;
 Serial.println("now:");
Serial.println(now);
// Speed=7812-counti*7812/500;
Speed=7812;
 setInterrupt1();
      }
void synch (void)
{

}
      
      
void ISR2(void)//inte
{

}

void setTX(void)
{  

  CC110L.GDO_Set();
  CC110L.Reset();
  CC110L.RegConfig();
  CC110L.PATABLE();

// SpiWriteReg(0x09,0x10);
//CC110L.WriteSingleReg(0x09,0x09);
//CC110L.WriteSingleReg(0x06,0xC9);
//CC110L.ReadBurstReg(0,RegBuffer,47);

}
void setRx(void)
{   

  CC110L.GDO_Set();
  CC110L.Reset();
  CC110L.RegConfig();
  //CC110L.PATABLE();
  CC110L.SetReceive();
}
void toggle()
{ if(light==0)
     {
  digitalWrite(6,HIGH);
     light=1;
      }
     else
     {
     digitalWrite(6,LOW);
     light=0;
     }
}
