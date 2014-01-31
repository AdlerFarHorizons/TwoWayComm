
#include <Wire.h>
byte x;
void setup()
{
  Wire.begin(5);//5 is going to be the id of this slave
   x=0;
  Wire.onRequest(requestEvent);
  
}

void loop()
{ x++;
  delay(500);
}

void requestEvent()
{
 // byte data[3];
  //data[0]=x;
//  data[1]=x/3;
 // data[2]=x%data[1]*3;*/
  Wire.write(x);
  //Wire.write(data[1]);
  //Wire.write(data[2]);
}

