#include <LCD5110_Basic.h>

//Inicializacion del lcd
LCD5110 Lcd(8,9,10,11,12);

extern uint8_t SmallFont[];
long contador;

void setup() {
  // put your setup code here, to run once:
  Lcd.InitLCD();
  Lcd.setFont(SmallFont);
  contador=0;
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Lcd.clrScr();
  Lcd.print("Contador:",CENTER,0);
  Lcd.print(String(contador),RIGHT,16);
  Serial.print("Contador LCD: ");
  Serial.println(contador);
  contador++;
  delay(5000);
}
