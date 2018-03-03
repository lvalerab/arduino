#include <LCD5110_Basic.h>

LCD5110 Lcd(8, 9, 10, 12, 11);

extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];

void setup() {
  // put your setup code here, to run once:
  Lcd.InitLCD();
  //Lectura del serial
  Serial.begin(9600);
  Serial1.begin(9600);
}

String LeeDatosSerial() {
  String linea = "";
  int contseg=0;
  if (Serial1.available() > 0) {
    char caracter; 
    while(caracter!='\n' && contseg<100) {
      char caracter = Serial1.read();
      linea += caracter;
      contseg++;
    }
  }
  if(contseg>=100) {
    Serial.println("Posible error al leer la linea");
  } else {
    Serial.println("Linea leida "+linea);
  }
  return linea;
}

void Datos(String cadena, String *retorno) {
  int pos = 0;
  int enc = 0;
  for (int i = 0; i < 10; i++) {
    retorno[i] = "";
  }
  for (int i = 0; i < cadena.length(); i++) {
    if (cadena.charAt(i) == '#') {
      pos++;
    } else {
      retorno[pos] = retorno[pos] + cadena.charAt(i);
    }
  }
}

void loop() {
  Serial.println("Esperando lectura de datos");
  String lectura = LeeDatosSerial();
  String dat[10];
  Datos(lectura, dat);
  Lcd.setFont(SmallFont);
  Lcd.clrScr();
  Lcd.print("Temp    /  Hum", CENTER, 0);
  Lcd.print("  " + dat[0] + "º / " + dat[1] + "%", CENTER, 16);
  Lcd.print("Hum Sue 1 /  Hum Sue 2", CENTER, 24);
  Lcd.print(" " + dat[2] + " / " + dat[3], CENTER, 32);
  Lcd.print("Nivel cuba: " + dat[4], CENTER, 40);
  Serial.println("Temp    /  Hum");
  Serial.println("  " + dat[0] + "º / " + dat[1] + "%");
  Serial.println("Hum Sue 1 /  Hum Sue 2");
  Serial.println(" " + dat[2] + " / " + dat[3]);
  Serial.println("Nivel cuba: " + dat[4]);
  delay(1000);
  Serial.println("Lectura de datos finalizada");
}
