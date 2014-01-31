#include <SoftwareSerial.h>

#include <TinyGPS.h>
#include <Serial.h>
SoftwareSerial softserial(10, 11);
TinyGPS gps;
void setup()
{
  Serial.begin(9600);
  softserial.begin(4800);
  Serial.println("start");
}
void loop()
{int got=0;
  while (softserial.available())
{got=1;

  Serial.println("available");
  int c = softserial.read();
  gps.encode(c);
  
}
long lat, lng;
gps.get_position(&lat, &lng, NULL);
if(got){
Serial.print("long");
Serial.println(lng);
Serial.print("lat");
Serial.println(lat);
}
}

