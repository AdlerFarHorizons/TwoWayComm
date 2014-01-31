#include <SPI.h>
#include <RADIO.h>
 
 
//byte RX_Buffer[61]={0};
//byte size,i;
//int rssiDec;
//int rssiOffset = 74;
int rssiDbm;
float freqChan0, deltaChan;
unsigned long freqWord;
float freqScale = 2520.615384615;
int rssiVals[256];
int delta[256];
unsigned long time, startTime;
 
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
  // FREQ = (int)(2520.615384615 * fchan0_mhz)
  freqChan0 = 420.0;
  deltaChan = 0.09620667;
  freqWord = (long)( freqScale * freqChan0 + 0.5 ); // 0.5 for rounding

  CC110L.WriteSingleReg(RADIO_MDMCFG4, 0x33 ); // channel bw = 58 kHz
  CC110L.WriteSingleReg(RADIO_DEVIATN, 0x15 ); // deviation = 5.2 kHz
  CC110L.WriteSingleReg(RADIO_FREQ2, freqWord >> 16 & 0x000000ff ); 
  CC110L.WriteSingleReg(RADIO_FREQ1, freqWord >>  8 & 0x000000ff ); 
  CC110L.WriteSingleReg(RADIO_FREQ0, freqWord & 0x000000ff );
  
}


void loop()
{
  Serial.flush();
  for ( int j = 0 ; j < 1 ; j++ ) {
    time = micros();
    startTime = time;
    delay(6);
    for ( int i = 0 ; i < 256 ; i++ ) {
      for (int j = 0 ; j < 16 ; j++ ) {
        CC110L.SetReceive();
        byte ibyte = byte(i);
        CC110L.WriteSingleReg(RADIO_CHANNR, ibyte );
        delay(10);
        rssiVals[i] += (int)CC110L.ReadStatusReg(RADIO_RSSI);
        delta[i] += (int)( micros() - time );
        time = micros();
      }
    }
  }
  Serial.print("measurement time: ");Serial.println( micros() - startTime );
  for ( int i = 0 ; i < 256 ; i++ ) {
    Serial.print((float)i * deltaChan + freqChan0);Serial.print(" ");
    Serial.print( delta[i] / 16 );Serial.print(" ");
    Serial.println( getRSSIdBm( rssiVals[i] / 16 ) );
  }
  while(true);
}

int getRSSIdBm( int rssiVal ) {
  if ( rssiVal >= 128 ) {
    return ( rssiVal - 256 ) / 2 - 74;
  } else {
    return rssiVal / 2 - 74;
  }
}











