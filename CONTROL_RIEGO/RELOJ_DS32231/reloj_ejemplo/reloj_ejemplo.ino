#include <Wire.h>
#define DS3231_I2C_ADDRESS 0x68

byte decToBcd(byte val) {
  return (val/10*16)+(val%10);
}

byte bcdToDec(byte val) {
  return (val/16*10)+(val%16);
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  setDS3231time(0, 5, 19, 1, 15, 2, 15);
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year) {
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); //Establezco el canal de comunicacion
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();
}

void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year) {
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); //Iniciamos el canal
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS,7); //Solicitamos 7 bytes del dispositivo en desde el registro 00h
  *second=bcdToDec(Wire.read() & 0x7f);
  *minute=bcdToDec(Wire.read());
  *hour=bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek=bcdToDec(Wire.read());
  *dayOfMonth=bcdToDec(Wire.read());
  *month=bcdToDec(Wire.read());
  *year=bcdToDec(Wire.read());
}

void displayTime() {
  byte s, m, h, dow, dom, mo, y;
  readDS3231time(&s, &m, &h, &dow, &dom, &mo, &y);
  Serial.print(h,DEC);
  Serial.print(":");
  if(m<10) {
    Serial.print("0");
  }
  Serial.print(m);
  Serial.print(":");
  if(s<10) {
    Serial.print("0");
  }
  Serial.print(s);
  Serial.print(" ");
  if(dow<10)
    Serial.print("0");
  Serial.print(dow, DEC);
  Serial.print("/");
  if(mo<10)
    Serial.print("0");
  Serial.print(mo, DEC);
  Serial.print("/");
  Serial.print(y, DEC);
  Serial.print(" Día: ");
  switch(dow){
    case 1:
      Serial.print("Domingo");
      break;
    case 2:
      Serial.print("Lunes");
      break;
    case 3:
      Serial.print("Martes");
      break;
    case 4:
      Serial.print("Miercoles");
      break;
    case 5:
      Serial.print("Jueves");
      break;
    case 6:
      Serial.print("Viernes");
      break;
    case 7:
      Serial.print("Sábado");
      break;
   }
   Serial.println(" ");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  displayTime();
  delay(5000);
}
