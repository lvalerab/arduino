#include "LFVBTFT.h"

utft miPantalla=new utft(A0,A1,A2,A3,A4);

void setup() {
  // put your setup code here, to run once:
  miPantalla->InitDPin();
  miPantalla->InitAPin();
  miPantalla->Init();
  miPantalla->Clear(0x00);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<1000;i++)
  {
    miPantalla->Rectangulo(random(300),random(300),random(300),random(300),random(65535)); // rectangle at x, y, with, hight, color
  }
  miPantalla->Clear(0x00);
}
