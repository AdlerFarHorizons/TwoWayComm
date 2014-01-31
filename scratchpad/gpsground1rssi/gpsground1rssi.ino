#include <queueNode.h>

//Timer interrupt Test trial
//Jeididah Oyeyemi
//David  Leibowitz
//june 29 2013
#include <SPI.h>
#include <RADIO.h>
#define maxi 10
#define pL 50    //packetlength
                                        /******GLOBAL VARIABLES***********/
/**************************************************************************************************************/

/*Time monitoring variables */
int act;  // variable to store the value coming from the sensor
int  Speed=7812;//the variable that determines the frequency of the  ISR1 16mhz/(1024*7812)=2.000013hz  roughly 2hz
unsigned long isc=0;//That increases everytime the timer interrupt is activated
unsigned long seconds=0;//a counter that increases every second by dividing the isc counter by 2
/*************************************************/




/*Radio data memory allocation*/
byte RegBuffer[50]={0};
Node TX_Buffer[maxi];//tranmisson buffer
Node RX_Buffer[maxi];//reception buffer
byte TXS[maxi][pL];//memory space allocation for the buffers
byte RXS[maxi][pL];//""""""""""""""""" 
/******************************************/



int light,light1;// status light monitors


/*Buffer Status moniroring variables*/
int  rpi=0; //the next dequeable  index of rx
int  rsi=0; //the next enquable index of rx
int  tpi=0; //the next dequeable  index of tx
int  tsi=0; //the next dequeable  index of tx
int  emT=1; //indicates the tx buffer is empty
int  fulR=0; //indicates the rx buffer is full
int  emR=1;//indcates if the rx bufer is empty
int  fulT=0;//indicates if the rx buffer is full
int tsp=0;//Indexes for allocated space to the buffers
int rsp=0;//indexes for space allocated to the buffers
/************************************************/

/*Frequency  and Power settings*/
int freq2 = 0x10;
int freq1 = 0xEC;
int freq0 = 0x4F;
byte paTable[8]= {0xC0, 0, 0, 0, 0, 0, 0, 0};
/**************************************************/

/*Radio data processing variables*/
byte source;//stores the destination or source of a reieved source
byte message;
byte * currentr;//points to a reiceved packet that is to be processed
/***************************************************************/

// For RSSI function (LN)
int rssiDec;
int rssiOffset = 74;
int rssiDbm;

void setup (void)
{
  Serial.begin(115200);
  
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(7,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);

  light=0;
  light1=0;
  setSPI();
  //setFrequency( freq2, freq1, freq2 ); 
  setInterrupt1(); 
  setRX();
}


void loop (void)
{
  while(1)
  {
    updateT();//updating the status of the trasmission buffer
    if(!emT)//if the transmission buffer is not empty this means only transmits when there is something to transmit
    {
      setTX();//goes to TX mode 
      noInterrupts();// makes sure transmission is not interrupted      
      CC110L.SendData(TX_Buffer[tpi].content,TX_Buffer[tpi].size);      
     // TX(TX_Buffer[tpi].content);// getting the current  popable transmission buffer  member
      Serial.println("sent");
      setRX();//goes back to recieve mode
      interrupts();// enabling interrupts        
        
     }
     emT=dequeueT();//updating the TX indexes for an effective data popping
     if(CC110L.CheckReceiveFlag())// Checking if there is data recieved
     {
       rssiDbm = getRSSI();
       int size=CC110L.ReceiveData(RXS[rsp]);// recieves the data
       updateR();//updating the status of the buffer
       if(!fulR)
       {
         RX_Buffer[rsi].content=RXS[rsp];//pushing the recieved data into the queue 
         RX_Buffer[rsi].size=size;// also recording its size
       }
       rsp=inc(rsp);//going to the next allocated space for recieved signals
       fulR=enqueueR();// updating the RX indexes for an effective data pushing
       CC110L.SetReceive();//setting back to recieve mode 
       //Serial.println("got");
     }
 
  if(source==1){
    source=0;
      byte *temp= &currentr[2];
      float * x= (float*)temp;
      float y=*x;
      x= (float*)&temp[4];
      float z=*x;
      Serial.print("RSSI ");
      Serial.print(rssiDbm);    
      Serial.print(" latt ");
      Serial.print(y,8);
      Serial.print(" lon  ");
      Serial.println(z,8);
  }
  if(source==2)
  {
    Serial.print("Temp: ");
    Serial.println(message);
    source=0;

  }
  if(source==3)
  {
    Serial.print("SPI");
    Serial.print(currentr[1]);
      Serial.print(":");
    Serial.println(message);
    source=0;

  }


  }
}//end of loop





/*********************I/O SOUBROUNTINES**************************/
void debouncer(int x)
{
  delay(10);
  while( digitalRead(x)==LOW)
  {// Serial.print("insidedebouncer");
  }
  delay(10);

}

void toggle()
{ 
  if(light==0)
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
void toggle1()
{ 
  if(light1==0)
  {
    digitalWrite(5,HIGH);
    light1=1;
  }
  else
  {
    digitalWrite(5,LOW);
    light1=0;
  }
}            

/********************* SETUP SUBROUTINES**************** */
void setSPI(void)
{
  SPI.begin ();
  SPI.setClockDivider(SPI_CLOCK_DIV4);   
  SPI.setBitOrder(MSBFIRST);   
  SPI.setDataMode(SPI_MODE0) ;

}

void setInterrupt1(void)//sets up the conditions of interrupt1
{  
  noInterrupts();           // disable all interrupts
 
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
void setTX(void)
{  

  CC110L.GDO_Set();
  CC110L.Reset();
  CC110L.RegConfig();
  //CC110L.PATABLE(); 
  CC110L.WriteBurstReg( 0x3E, paTable, 8 ); 
  CC110L.ReadBurstReg(0,RegBuffer,47);


}


void setRX(void)
{   

  CC110L.GDO_Set();
  CC110L.Reset();
  CC110L.RegConfig(); 
  CC110L.SetReceive();
}
/********************* TX/RX************************************************/
ISR(TIMER1_COMPA_vect)//first timer interrupt
{ 
  toggle1();
  updateR();//updatiing the space status on the  Rx biffer
  if(!emR)//if RX Buffer is not empty pops what is on it 
  {
    //Serial.println("rx");
    source= RX_Buffer[rpi].content[0];
    message=RX_Buffer[rpi].content[2];
    currentr=RX_Buffer[rpi].content;
  }   
  emR=dequeueR();//popping of the next item on RX buffer 
  if(digitalRead(7)==LOW)//detecting button push
  {  
    debouncer(7);//bebouncing the button
    TXS[tsp][0]=4;//setting up the packet
    TXS[tsp][1]=0;
    TXS[tsp][2]=1; 
    updateT();
    if(!fulT)//putting the packet in the queue
    {
      TX_Buffer[tsi].content=TXS[tsp];
      TX_Buffer[tsi].size=3;
    }
    tsp=inc(tsp);
    fulT=enqueueT();
  }
  if(digitalRead(4)==LOW)//another button detection
  { 
    debouncer(4);
    //input=(float)digitalRead(1);
    //input=input/4.0;
    TXS[tsp][0]=4;
    TXS[tsp][1]=0;
    TXS[tsp][2]=2; 
    updateT();
    if(!fulT)
    {
      TX_Buffer[tsi].content=TXS[tsp];
      TX_Buffer[tsi].size=3;
    }
    tsp=inc(tsp);
    fulT=enqueueT();   

  }
}


/******************************************************queue routines********************/
int enqueueT(void)
{  
  if(tsi!=tpi)
    fulT=0;
  if(!fulT)
  {//Serial.print("push:") ;
    //Serial.println(tsi) ;
    tsi=inc(tsi);
  }
  if(tsi==tpi)
  {//Serial.println("fullT");
    return 1;//return 0 if the queue is full
  }
  else return 0;//returning 1 if the queue is not full
}




int dequeueT(void)
{
  if(tsi!=tpi)
    emT=0;
  if(!emT)
  {
    // Serial.print("pop:") ;
    //Serial.println(tpi) ;
    tpi=inc(tpi);
  }

  if(tsi==tpi)
  {
    return 1;//return 1 if the queue is empty
  }
  else return 0;//returning 0 if the queue is not empty
}



int enqueueR()
{
  if(!fulR)
    rsi=inc(rsi);
  if(rsi==rpi)
  {
    return 1;//return 1 if the queue is full
  }
  return 0;
}


int dequeueR()
{
  if(!emR)
  {
    rpi=inc(rpi);
  }
  if(rsi==rpi)
  { 
    return 1;//return 1 if the queue is empty
  }
  else return 0;//returning 0 if the queue is not empty
}

int inc(int x)
{
  if(x==(maxi-1))
    x=0;
  else x+=1;
  return x;
}
void updateT(void)
{
  if(tpi!=tsi)
  {
    fulT=0;
    emT=0;
  }
}
void updateR(void)
{
  if(rpi!=rsi)
  {
    fulR=0;
    emR=0;
  }
}
void setFrequency(int f2, int f1, int f0 )
        {
                CC110L.WriteSingleReg( 0x0D, f2 );
                CC110L.WriteSingleReg( 0x0E, f1 );
                CC110L.WriteSingleReg( 0x0F, f0 );
        }

int getRSSI()
  { 
    rssiDec = (int)CC110L.ReadStatusReg(RADIO_RSSI);
    if(rssiDec >=128)
      {
        return (rssiDec-256)/2-rssiOffset;
      }
      else
      {
        return (rssiDec)/2-rssiOffset;
      }
  }    
