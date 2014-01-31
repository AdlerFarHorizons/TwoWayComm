#include "SPI.h"

#define MOSI 11
#define MISO 12
#define SPICLOCK 13 //sck
#define BUFFSIZE 5


byte buffer[BUFFSIZE];
byte data;
//boolean just;

void setup()
{
  data = 11;
  //just = true;
  for(int i = 0; i < BUFFSIZE; i++){ buffer[i] = data; }
  
  pinMode(MISO, OUTPUT);
    pinMode(10, INPUT);
  Serial.begin(9600);
  SPCR |= _BV(SPE);
  
  SPI.attachInterrupt();
}

void loop()
{//Serial.println("running"); 
}

// SPI Interrupt
ISR(SPI_STC_vect)
{
 // for(int i = 0; i < BUFFSIZE; i++)
  //{
    SPDR = 5;
    while (!(SPSR & _BV(SPIF)));
      
  //}
  data++;
 // for(int i = 0; i < BUFFSIZE; i++)
  {
 //   buffer[i] = 27; 
  }
}
