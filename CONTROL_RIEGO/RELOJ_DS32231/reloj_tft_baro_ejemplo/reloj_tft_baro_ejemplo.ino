#include <dht.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

//#include <TFT.h>

//estructuras

struct DateTime {
  byte second;
  byte minute;
  byte hour;
  byte dayOfWeek;
  byte dayOfMonth;
  byte month;
  byte year;
};

struct DatTiempo {
  double presion;
  double temperatura;
  double humedad;  
};

struct RegTiempo {
   DateTime hora;
   DatTiempo parametros; 
};

//DHT11
#define PIN_DHT11 A8

//I2C direcciones
#define DS3231_I2C_ADDRESS 0x68

//SPI TFT access
#define TFT_SCLK 52
#define TFT_MOSI 51
#define TFT_CS 53
#define TFT_DC 50
#define TFT_RS 48


Adafruit_ST7735 miPantalla = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RS);
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
dht DHT;

//Funciones DHT
void InitDHT() {
  DHT.read11(PIN_DHT11);
}


float TemperaturaDHT() {
  return DHT.temperature;
}

float HumedadDHT() {
  return DHT.humidity;
}

void ImprimeDHT() {
   InitDHT();
   float t=TemperaturaDHT();
   float h=HumedadDHT();
   Serial.println("DHT values");
   Serial.print("Temperatura: ");
   Serial.println(t);
   Serial.print("Humedad: ");
   Serial.println(h);
    miPantalla.setCursor(0,40);
    miPantalla.print("DHT values");
    miPantalla.setCursor(0,50);
    miPantalla.print("Temperatura: ");
    miPantalla.print(t);
    miPantalla.print(" C");
    miPantalla.setCursor(0,60);
    miPantalla.print("Humedad "); 
    miPantalla.print(h);
   miPantalla.print(" %"); 
}

//TFT miPantalla=new TFT(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RS);;

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
  miPantalla.initR(INITR_BLACKTAB);
  Serial.println("Inicializado TFT");
  setDS3231time(20, 45, 0, 1, 22, 2, 15);
  if(!bmp.begin()) {
    Serial.println("No se ha podido detectar el sensor de presion");
    miPantalla.fillScreen(ST7735_RED); 
    miPantalla.setTextColor(ST7735_YELLOW);
    miPantalla.setCursor(0,0);
    miPantalla.print("S. Presion Off");
  } else {
      miPantalla.fillScreen(ST7735_BLACK); 
      miPantalla.setTextColor(ST7735_GREEN);
      miPantalla.setCursor(0,0);
      miPantalla.print("S. Presion On");
  }
  delay(1000);
}

//void setDS3231timeStr(DateTime hora) {
//  setDS3231time(hora.second, hora.minute, hora.hour, hora.dayOfWeek, hora.dayOfMonth, hora.month, hora.year);
//}

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
  miPantalla.setCursor(0, 0);
  miPantalla.fillScreen(ST7735_BLACK); 
  miPantalla.setTextColor(ST7735_GREEN);
  //miPantalla.setTextSize(3);
  miPantalla.setTextWrap(true);
  readDS3231time(&s, &m, &h, &dow, &dom, &mo, &y);
  Serial.print(h,DEC);
  miPantalla.print(h);
  Serial.print(":");
  miPantalla.print(":");
  if(m<10) {
    Serial.print("0");
    miPantalla.print("0");
  }
  Serial.print(m);
  miPantalla.print(m);
  Serial.print(":");
  miPantalla.print(":");
  if(s<10) {
    Serial.print("0");
    miPantalla.print("0");
  }
  Serial.print(s);
  miPantalla.print(s);
  Serial.print(" ");
  miPantalla.print(" ");
//  miPantalla.setCursor(0, 30);
  if(dow<10)
    Serial.print("0");
    miPantalla.print("0");
  Serial.print(dow);
  miPantalla.print(dow);
  Serial.print("/");
  miPantalla.print("/");
  if(mo<10)
    Serial.print("0");
    miPantalla.print("0");
  Serial.print(mo);
  miPantalla.print(mo);
  Serial.print("/");
  miPantalla.print("/");
  Serial.print(y);
  miPantalla.print(y);
  Serial.print(" Día: ");
  miPantalla.setCursor(0, 10);
  miPantalla.print(" Dia: ");  
  switch(dow){
    case 1:
      Serial.print("Domingo");
      miPantalla.print("Domingo");
      break;
    case 2:
      Serial.print("Lunes");
      miPantalla.print("Lunes");
      break;
    case 3:
      Serial.print("Martes");
      miPantalla.print("Martes");
      break;
    case 4:
      Serial.print("Miercoles");
      miPantalla.print("Miercoles");
      break;
    case 5:
      Serial.print("Jueves");
      miPantalla.print("Jueves");
      break;
    case 6:
      Serial.print("Viernes");
      miPantalla.print("Viernes");
      break;
    case 7:
      Serial.print("Sábado");
      miPantalla.print("Sábado");
      break;
   }
   Serial.println(" ");
}

void ImprimePresion() {
  sensors_event_t event;
  bmp.getEvent(&event);
  
  if(event.pressure) {
      Serial.print("Presion: ");
      Serial.print(event.pressure);
      Serial.println(" hPa");
      miPantalla.setCursor(0,20);
      miPantalla.print("Presión: ");
      miPantalla.print(event.pressure);
      miPantalla.print(" hPa"); 
      miPantalla.setCursor(0,30);
      float temperatura;
      bmp.getTemperature(&temperatura);
      Serial.print("Temperatura: ");
      Serial.print(temperatura);
      Serial.println(" C");
      miPantalla.print("Temperatura: ");
      miPantalla.print(temperatura);
      miPantalla.print(" C");      
  } else {
    Serial.print("Error leyendo la presion");
    miPantalla.setCursor(0,70);
    miPantalla.print("Error leyendo la presion");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  displayTime();
  delay(250);
  ImprimePresion();
  delay(250);
  ImprimeDHT();
  delay(1200);
}
