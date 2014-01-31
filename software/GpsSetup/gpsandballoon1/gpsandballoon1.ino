#include <queueNode.h>

//2way comm final designs sky Aug 8 flight Version 1.
//Jeididah Oyeyemi
//July, 31, 2013
#include <SPI.h>
#include <RADIO.h>

#include <Wire.h>
#define maxi 10  //buffer soze
#define pL 61    //Maxumimum packet length
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
byte * current;//points to a reiceved packet that is to be processed
/***************************************************************/


/*some sample automatic periodic transmission flags*/
char auto1=0;
char auto2=0;
char auto3=0;
char allow=0;
/*****************************************/


void setup (void)
{
  Serial.begin(115200);
  Wire.begin();
  light=0;
  light1=0;
  setSPI();
  //setFrequency( freq2, freq1, freq2 );
  Speed=7812;
  setInterrupt1();
  setRX(); //starts in receive mode
}


void loop (void)
{    
    updateT();//updating the status of the trasmission buffer
    if(!emT)//if the transmission buffer is not empty this means only transmits when there is something to transmit
    {
      setTX();//goes to TX mode 
      noInterrupts();// makes sure transmission is not interrupted      
      CC110L.SendData(TX_Buffer[tpi].content,TX_Buffer[tpi].size);      
      // TX(TX_Buffer[tpi].content);// getting the current  popable transmission buffer  member
      Serial.println(TX_Buffer[tpi].content[1]);
      setRX();//goes back to recieve mode
      interrupts();// enabling interrupts        
        
     }
     emT=dequeueT();//updating the TX indexes for an effective data popping
     
     if(CC110L.CheckReceiveFlag())// Checking if there is data recieved
     {
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
      //Serial.println(size);
      //Serial.println("got");
     }
     
 
     if(auto1==1) //checking  if there is a message from source flag
     {  
       auto1=0;// sets the source back to zero
       Serial.println("latt");  
         
       TXS[tsp][0]=1;//Identifying itself at the beginning of  the  packet the first byte is for source
       TXS[tsp][1]=10;
       byte * temp= &TXS[tsp][2];       
       collect2(5,8,temp);
       stackT(10);
       float * x= (float*)temp;
       float y=*x;
       x= (float*)&temp[4];
       float z=*x;
       Serial.print("latt ");
       Serial.println(y,8);
       Serial.print("lon  ");
       Serial.println(z,8);
     }
    
   
 }
 ISR(TIMER1_COMPA_vect)//first timer interrup
{
  updateR();//updatiing the space status on the  Rx biffer
  if(!emR)//if RX Buffer is not empty pops what is on it 
  {
    //Serial.println("split"); 
    if(( RX_Buffer[rpi].content[0]==4))//4 is for request
    source= RX_Buffer[rpi].content[2];//getting the destination of the request
  }
  emR=dequeueR();//popping of the next item on RX buffer
  
  /*below is the second way of handling recieved data this on is located inside the ISR Because it does not use the I2C interface **/
  /* if(source==1)//checking the source for the request
  {
    source=0;// resetting the source
    if(allow==1)
    allow=2;
    else allow=1;
  }
  if(source==2)//checking the source for the request
  {
    source=0;// resetting the source
    if(allow==0)
    allow=1;
    else allow=0;
  }
 /****periodic transmissions ***/ 
if(isc%20==0)
  {auto1=1;
  Serial.print("auto1 int") ;
  }

  isc++;//updating the time
  seconds=isc/2;
}
 
 
 
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
void toggle1()
{ if(light1==0)
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
  for(int i = 0; i <4; i++)
  {
    pinMode(i, OUTPUT); // Experiment pins
    digitalWrite(i, HIGH); // Disable transfer for all of them
    // delay (1000);
  }
  SPI.begin ();
  SPI.setClockDivider(SPI_CLOCK_DIV4);   
  SPI.setBitOrder(MSBFIRST);   
  SPI.setDataMode(SPI_MODE0);
}

void setInterrupt1(void)//sets up the conditions of interrupt1
{  
  noInterrupts();           // disable all interrupts
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = Speed;// = (16*10^6) / (1*2*1024) - 1 (must be <65536)
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
  // SpiWriteReg(0x09,0x10);
  //CC110L.WriteSingleReg(0x09,0x09);
  //CC110L.WriteSingleReg(0x06,0xC9);
  CC110L.ReadBurstReg(0,RegBuffer,47);
}


void setRX(void)
{
  CC110L.GDO_Set();
  CC110L.Reset();
  CC110L.RegConfig();
  //CC110L.PATABLE();
  CC110L.SetReceive();
}
/********************* TX/RX************************************************/


/*********Sync routines*******/

void TX(byte *TX_BFF)
{
  setTX();
  CC110L.SendData(TX_BFF,3);
}

     /******************************************************queue routines********************/
int enqueueT(void)
{
  if(tsi!=tpi)
  fulT=0;
  if(!fulT)
{//Serial.print("push:") ;
 //Serial.println(tsi) ;
tsi=inc(tsi);}
  if(tsi==tpi)
  {//Serial.println("fullT");
    return 1;//return 0 if the queue is full
   }
   else return 0;//returning 1 if the queue is not full
 }
 
 
 
 
 int dequeueT(void)
 {if(tsi!=tpi)
      emT=0;
  if(!emT){
   // Serial.print("pop:") ;
 //Serial.println(tpi) ;
  tpi=inc(tpi);
}

  if(tsi==tpi)
  {// Serial.println("emptyT");
    return 1;//return 0 if the queue is empty
   }
  else return 0;//returning 1 if the queue is not empty
 }
 
 
 
 int enqueueR()
 {
   if(!fulR)
  rsi=inc(rsi);
  if(rsi==rpi)
  {//Serial.println("fullR");
    return 1;//return 0 if the queue is full
    }
  return 0;
 }
 
 
 int dequeueR()
 {
  if(!emR)
  {//Serial.println("rpi?");
    rpi=inc(rpi);
}
  if(rsi==rpi)
  { //Serial.println("emptyR");
   return 1;//return 0 if the queue is empty
   }
  else return 0;//returning 1 if the queue is not empty
 }
 
 
 int inc(int x)
 {if(x==(maxi-1))
   x=0;
  else x+=1;
  return x;
 }
 
 
 
byte collect(int expNum)
{
  byte result;
  digitalWrite(expNum, LOW);
  //delay(500);
   //Serial.print("collecting!");
  //  Serial.println(expNum);
  //for(int i = 0; i < BUFFSIZE; i++)
  {
    result= SPI.transfer(0);
  }
  digitalWrite(expNum, HIGH);
 // delay(500);
// Serial.println("done!");
 return result;
}

/*byte collect2(int experiment,int y)
{
  byte c;
  Wire.requestFrom(experiment,y,); 
  while(Wire.available())
  {
    c = Wire.read();
  //  Serial.println(c);
  }
  return c;
}*/
byte collect2(int experiment,int length, byte * bff)
{
  byte c;
  Wire.requestFrom(experiment,length); 
  int count=0;
  while(Wire.available())
  {
  c=Wire.read();
  bff[count] = c;
  count++;
   //Serial.println(c);
  }
  return c;
}
void updateT(void)
{if(tpi!=tsi)
{
 fulT=0;
 emT=0;
}
}
void stackT(int length)
{
   updateT();//updating buffer status  of the trasmission buffer
       if(!fulT)//checking if the buffer
       {
         TX_Buffer[tsi].content=TXS[tsp];//pushing the packet onto the buffer
         TX_Buffer[tsi].size=length;
         //Serial.println("pushed1");
       }
       tsp=inc(tsp);//updating the allocated TX space index
       fulT=enqueueT(); //updating the TX buffer status  and indexes 
}
void updateR(void)
{if(rpi!=rsi)
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
        
