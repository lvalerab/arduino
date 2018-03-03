//Definicion nº pasos grafica
#define _GRAPH_PASS 40
#define _GRAPH_ANCH 5
#define _PASS_REG_GRAF 2

//Puertos
//DHT
#define pdDHT 13
//fotoresistencia
#define paFTR A0

//LCD
#define TFT_SCLK 12
#define TFT_MOSI 11
#define TFT_CS 9
#define TFT_DC 10
#define TFT_RS 8

//BMP
//define I2C_DIR_BMP

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>

#include <dht.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>


dht DHT;

Adafruit_ST7735 miPantalla = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RS);

Adafruit_BMP085 bmp;

float a_temp[_GRAPH_PASS];
float a_pr[_GRAPH_PASS];
float a_hum[_GRAPH_PASS];
float a_lum[_GRAPH_PASS];
float a_pres[_GRAPH_PASS];
int hora=0;
int seg=0;

void InitMat() {
  for (int i=0;i<_GRAPH_PASS;i++) {
    a_temp[i]=0;
    a_pr[i]=0;
    a_hum[i]=0;
    a_lum[i]=0;
    a_pres[i]=950;
  }
}


void setup() {
  // put your setup code here, to run once:
  miPantalla.initR(INITR_BLACKTAB);
  miPantalla.setRotation(1);
  InitMat();
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
  Serial.println("Empezamos...");  
}

void LCDColorTextoT(float &valor) {
  if (valor < 0) {
    miPantalla.setTextColor(ST7735_BLUE);
  } else if (valor >= 0 && valor < 10) {
    miPantalla.setTextColor(ST7735_YELLOW);
  } else if (valor >= 10 && valor < 20) {
    miPantalla.setTextColor(ST7735_GREEN);
  } else if(valor>=20 && valor<30) {
    miPantalla.setTextColor(ST7735_YELLOW);
  } else 
  {
    miPantalla.setTextColor(ST7735_RED);
  }
}

void LCDColorTextoH(float &valor) {
  if(valor<30) {
    miPantalla.setTextColor(ST7735_RED);
  } else if(valor>=30 && valor<60) {
    miPantalla.setTextColor(ST7735_GREEN);
  } else if(valor>=60 && valor<90) {
    miPantalla.setTextColor(ST7735_YELLOW);
  } else {
    miPantalla.setTextColor(ST7735_BLUE);
  }
}

float ConvertPaTommHg(float &valor) {
  return (valor*1013.25)/101325;
}

float PuntoRocio(float &temp, float &hum) {
  return pow((hum/100),0.125)*(112+0.9*temp)+(0.1*temp)-112;
}

void LCDDatos(float &temp, float &hum, float &lum, float &press, float &temp01) {  
  miPantalla.fillScreen(ST7735_BLACK);
  miPantalla.setTextColor(ST7735_WHITE);
  miPantalla.setCursor(1, 1);
  miPantalla.print("======DHT=============");
  miPantalla.setCursor(1, 10);
  miPantalla.print("Temperatura ");
  LCDColorTextoT(temp);
  miPantalla.print(temp);
  miPantalla.setTextColor(ST7735_WHITE);
  miPantalla.print(" C");
  miPantalla.setCursor(1, 20);
  miPantalla.print("Humedad ");
  LCDColorTextoH(hum);
  miPantalla.print(hum);
  miPantalla.setTextColor(ST7735_WHITE);
  miPantalla.print(" %");
  miPantalla.setCursor(1, 30);
  miPantalla.print("Luz ");
  miPantalla.print(lum);
  miPantalla.print(" %");
  miPantalla.setCursor(1, 40);
  miPantalla.print("======BMP==============");
  miPantalla.setCursor(1, 50);
  miPantalla.print("Temperatura: ");
  LCDColorTextoT(temp01);
  miPantalla.print(temp01);  
  miPantalla.setTextColor(ST7735_WHITE);
  miPantalla.print(" C");
  miPantalla.setCursor(1, 60);
  miPantalla.print("Presion: ");
  miPantalla.print(ConvertPaTommHg(press));
  miPantalla.print(" mmHg");
  miPantalla.setCursor(1,70);
  miPantalla.print("======CALCULOS========");
  miPantalla.setCursor(1,80);
  miPantalla.print("P. Rocio ");
  miPantalla.print(PuntoRocio(temp01,hum));
  miPantalla.print(" C");

  miPantalla.setCursor(1,100);
  miPantalla.print("H ");
  miPantalla.print(hora);
  miPantalla.print("  S ");
  miPantalla.print(seg);
}

void ToGCoord (int &x, int &y) {
  x+=12;
  y+=8;
}

void GpTemp () {  
  miPantalla.fillScreen(ST7735_WHITE);
  miPantalla.setTextColor(ST7735_BLACK);
  miPantalla.setCursor(1,1);
  miPantalla.print("Temperatura (C) Ult:");
  miPantalla.setCursor(1,10);
  miPantalla.setTextColor(ST7735_RED);
  miPantalla.print(a_temp[hora]);
  miPantalla.setCursor(40,10);
  miPantalla.setTextColor(ST7735_BLUE);
  miPantalla.print(a_pr[hora]);
  for(int i=0;i<_GRAPH_PASS;i++) {
    int y=round(a_temp[i]*128/50);
    int yp=round(a_pr[i]*128/50);
    miPantalla.fillRect(_GRAPH_ANCH*i,128-y,_GRAPH_ANCH-1,y,ST7735_RED);
    miPantalla.fillRect(_GRAPH_ANCH*i,128-y,_GRAPH_ANCH-1,yp,ST7735_BLUE);
  }
}

void GpHum() {
  miPantalla.fillScreen(ST7735_WHITE);
  miPantalla.setTextColor(ST7735_BLACK);
  miPantalla.setCursor(1,1);
  miPantalla.print("% Humedad Ult: ");
  miPantalla.print(a_hum[hora]);
  for(int i=0;i<_GRAPH_PASS;i++) {
    int y=round(a_hum[i]*128/100);    
    miPantalla.fillRect(_GRAPH_ANCH*i,128-y,_GRAPH_ANCH-1,y,ST7735_BLUE);
  }
}

void GpLum() {
  miPantalla.fillScreen(ST7735_WHITE);
  miPantalla.setTextColor(ST7735_BLACK);
  miPantalla.setCursor(1,1);
  miPantalla.print("% Luz Ult: ");
  miPantalla.print(a_lum[hora]);
  for(int i=0;i<_GRAPH_PASS;i++) {
    int y=round(a_lum[i]*128/100);
    miPantalla.fillRect(_GRAPH_ANCH*i,128-y,_GRAPH_ANCH-1,y,ST7735_BLUE);
  }
}

void GpPres() {
  miPantalla.fillScreen(ST7735_WHITE);
  miPantalla.setTextColor(ST7735_BLACK);
  miPantalla.setCursor(1,1);
  miPantalla.print("Presión (P-940) Ult ");
  miPantalla.print(a_pres[hora]);
  for(int i=0;i<_GRAPH_PASS;i++) {  
    int y=round(((a_pres[i]-950))* (128/20));
    miPantalla.fillRect(_GRAPH_ANCH*i,128-y,_GRAPH_ANCH-1,y,ST7735_BLACK);
  }
}

void ReordenarArrays() {
  for(int i=0;i<_GRAPH_PASS-1;i++) {
    a_temp[i]=a_temp[i+1];
    a_pr[i]=a_pr[i+1];
    a_hum[i]=a_hum[i+1];
    a_pres[i]=a_pres[i+1]; 
    a_lum[i]=a_lum[i+1];
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  DHT.read11(pdDHT);
  float t = DHT.temperature;
  float h = DHT.humidity;
  float bt = 0, bp = 0;
  bt=bmp.readTemperature();  
  bp=bmp.readPressure(); 
  //Serial.println(ConvertPaTommHg(bp));
  //Serial.println(l);
  float l = (float)map(analogRead(paFTR), 0, 1023, 0, 100);
  Serial.println(l);
  LCDDatos(t, h, l, bp, bt);
  delay(1000);
  if(seg==_PASS_REG_GRAF) {
    seg=0;
    hora++;
    if(hora>_GRAPH_PASS) {
      ReordenarArrays();
      hora=23;
    }
  } else {
    seg++;
  }
   a_temp[hora]=bt;
   a_pr[hora]=PuntoRocio(bt,h);
   a_hum[hora]=h;
   a_pres[hora]=ConvertPaTommHg(bp); 
   a_lum[hora]=l;
   GpTemp ();
   delay(1000);
   GpHum ();
   delay(1000);   
   GpLum ();
   delay(1000);
   GpPres();
   delay(1000);
}
