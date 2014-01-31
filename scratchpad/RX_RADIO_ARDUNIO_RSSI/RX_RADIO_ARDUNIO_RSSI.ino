#include <SPI.h>
#include <RADIO.h>
 
 
 byte RX_Buffer[61]={0};
byte size,i;
int rssiDec;
int rssiOffset = 74;
int rssiDbm;
 
 void setup()
{
  Serial.begin (9600);
//  Serial.println ();
  
     

  
  SPI.begin ();
  SPI.setClockDivider(SPI_CLOCK_DIV4);   
  SPI.setBitOrder(MSBFIRST);   
  SPI.setDataMode(SPI_MODE0) ;
  CC110L.GDO_Set();
  CC110L.Reset();
  CC110L.RegConfig();
  //CC110L.PATABLE();
  CC110L.SetReceive();
  
}


void loop()
{
  //if(CC110L.CheckReceiveFlag())
  //{
    //size=CC110L.ReceiveData(RX_Buffer);
   // for(i=0;i<size;i++)
    //{
      //Serial.print(RX_Buffer[1],DEC);
//      Serial.print(' ',BYTE);
   // }
    //Serial.println("0");
    //CC110L.SetReceive();
    rssiDec = (int)CC110L.ReadStatusReg(RADIO_RSSI);
    if(rssiDec >=128)
    {
      rssiDbm = (rssiDec-256)/2-rssiOffset;
    }
    else
    {
      rssiDbm = (rssiDec)/2-rssiOffset;
    }
    Serial.println(rssiDbm);
    delay(1000);
  //}
}











