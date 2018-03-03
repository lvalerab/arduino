#include <dht.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

type DateTime {
 byte s;
 byte m;
 byte h;
 byte dw;
 byte dm;
 byte mm;
 byte y;
};

struct DatTiempo {
   DateTime tm;
   double presion;
   double temperatura;
   double humedad;
   double luminosidad; 
};

DateTime GetTime() {
  
}:



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
