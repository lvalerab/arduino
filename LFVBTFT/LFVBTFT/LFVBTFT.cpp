#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "LFVBTFT.h"

utft::utft(byte pRD, byte pWR, byte pRS, byte pCS, byte pREST) {
  utft::RD = pRD;
  utft::WR = pWR;
  utft::RS = pRS;
  utft::CS = pCS;
  utft::REST = pREST;
}

void utft::Write_Bus(char VH)
{
  unsigned char i, temp, data;
  data = VH;
  for (i = 8; i <= 9; i++)
  {
    temp = (data & 0x01);
    if (temp)
      digitalWrite(i, HIGH);
    else
      digitalWrite(i, LOW);
    data = data >> 1;
  }
  for (i = 2; i <= 7; i++)
  {
    temp = (data & 0x01);
    if (temp)
      digitalWrite(i, HIGH);
    else
      digitalWrite(i, LOW);
    data = data >> 1;
  }

  digitalWrite(utft::WR, LOW);
  digitalWrite(utft::WR, HIGH);
}

void utft::Write_Com(char VH)
{
  digitalWrite(utft::RS, LOW);
  utft::Write_Bus(VH);
}

void utft::Write_Data(char VH)
{
  digitalWrite(utft::RS, HIGH);
  utft::Write_Bus(VH);
}

void utft::Write_Com_Data(int com, int dat)
{
  utft::Write_Com(com);
  utft::Write_Data(dat);
}

void utft::Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
  utft::Write_Com(0x2a); // Set_column_address 4 parameters
  utft::Write_Data(x1 >> 8);
  utft::Write_Data(x1);
  utft::Write_Data(x2 >> 8);
  utft::Write_Data(x2);

  utft::Write_Com(0x2b); // Set_page_address 4 parameters
  utft::Write_Data(y1 >> 8);
  utft::Write_Data(y1);
  utft::Write_Data(y2 >> 8);
  utft::Write_Data(y2);

  utft::Write_Com(0x2c); // Write_memory_start
}

//Funcion para iniciar los pines donde se encuentra los pines digitales
//aqui hay que cambiarlo en caso de querer otros
void utft::InitDPin(void) {
  for (int p = 2; p < 10; p++)
  {
    pinMode(p, OUTPUT);
  }
}

void utft::InitAPin(void) {
  pinMode(utft::RD, OUTPUT);
  pinMode(utft::WR, OUTPUT);
  pinMode(utft::RS, OUTPUT);
  pinMode(utft::CS, OUTPUT);
  pinMode(utft::REST, OUTPUT);
  digitalWrite(utft::RD, HIGH);
  digitalWrite(utft::WR, HIGH);
  digitalWrite(utft::RS, HIGH);
  digitalWrite(utft::CS, HIGH);
  digitalWrite(utft::REST, HIGH);
}

void utft::Init(void)
{
  digitalWrite(utft::REST, HIGH);
  delay(5);
  digitalWrite(utft::REST, LOW);
  delay(15);
  digitalWrite(utft::REST, HIGH);
  delay(15);

  digitalWrite(utft::CS, HIGH);
  digitalWrite(utft::WR, HIGH);
  digitalWrite(utft::CS, LOW); //CS

  utft::Write_Com(0xE9);
  utft::Write_Data(0x20);

  utft::Write_Com(0x11); //Exit Sleep
  delay(100);

  utft::Write_Com(0xD1);
  utft::Write_Data(0x00);
  utft::Write_Data(0x71);
  utft::Write_Data(0x19);

  utft::Write_Com(0xD0);
  utft::Write_Data(0x07);
  utft::Write_Data(0x01);
  utft::Write_Data(0x08);

  utft::Write_Com(0x36);
  utft::Write_Data(0x48);

  utft::Write_Com(0x3A);
  utft::Write_Data(0x05);

  utft::Write_Com(0xC1);
  utft::Write_Data(0x10);
  utft::Write_Data(0x10);
  utft::Write_Data(0x02);
  utft::Write_Data(0x02);

  utft::Write_Com(0xC0); //Set Default Gamma
  utft::Write_Data(0x00);
  utft::Write_Data(0x35);
  utft::Write_Data(0x00);
  utft::Write_Data(0x00);
  utft::Write_Data(0x01);
  utft::Write_Data(0x02);

  utft::Write_Com(0xC5); //Set frame rate
  utft::Write_Data(0x04);

  utft::Write_Com(0xD2); //power setting
  utft::Write_Data(0x01);
  utft::Write_Data(0x44);

  utft::Write_Com(0xC8); //Set Gamma
  utft::Write_Data(0x04);
  utft::Write_Data(0x67);
  utft::Write_Data(0x35);
  utft::Write_Data(0x04);
  utft::Write_Data(0x08);
  utft::Write_Data(0x06);
  utft::Write_Data(0x24);
  utft::Write_Data(0x01);
  utft::Write_Data(0x37);
  utft::Write_Data(0x40);
  utft::Write_Data(0x03);
  utft::Write_Data(0x10);
  utft::Write_Data(0x08);
  utft::Write_Data(0x80);
  utft::Write_Data(0x00);

  utft::Write_Com(0x2A);
  utft::Write_Data(0x00);
  utft::Write_Data(0x00);
  utft::Write_Data(0x00);
  utft::Write_Data(0xeF);

  utft::Write_Com(0x2B);
  utft::Write_Data(0x00);
  utft::Write_Data(0x00);
  utft::Write_Data(0x01);
  //  utft::WRite_Data(0x3F);
  utft::Write_Data(0x8F); // on internet

  utft::Write_Com(0x29); //display on

  utft::Write_Com(0x2C); //display on

  digitalWrite(utft::CS, HIGH);
}

void utft::Punto(unsigned int x, unsigned int y, unsigned int c) {
  utft::Write_Com(0x02c);
  digitalWrite(utft::RS, HIGH);
  digitalWrite(utft::CS, LOW);
  utft::Address_set(x, y, x, y);
  utft::Write_Data(c);
  digitalWrite(utft::CS, HIGH);
}

void utft::HLine(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{
  unsigned int i, j;
  utft::Write_Com(0x02c); //write_memory_start
  digitalWrite(utft::RS, HIGH);
  digitalWrite(utft::CS, LOW);
  l = l + x;
  utft::Address_set(x, y, l, y);
  j = l * 2;
  for (i = 1; i <= j; i++)
  {
    utft::Write_Data(c);
  }
  digitalWrite(utft::CS, HIGH);
}

void utft::VLine(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{
  unsigned int i, j;
  utft::Write_Com(0x02c); //write_memory_start
  digitalWrite(utft::RS, HIGH);
  digitalWrite(utft::CS, LOW);
  l = l + y;
  utft::Address_set(x, y, x, l);
  j = l * 2;
  for (i = 1; i <= j; i++)
  {
    utft::Write_Data(c);
  }
  digitalWrite(utft::CS, HIGH);
}

void utft::Rectangulo(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
{
  utft::HLine(x  , y  , w, c);
  utft::HLine(x  , y + h, w, c);
  utft::HLine(x  , y  , h, c);
  utft::HLine(x + w, y  , h, c);
}

void utft::RectanguloF(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
{
  unsigned int i;
  for (i = 0; i < h; i++)
  {
    utft::line(x  , y  , w, c);
    utft::line(x  , y + i, w, c);
  }
}

void utft::Clear(unsigned int j)
{
  unsigned int i, m;
  utft::Write_Com(0x02c); //write_memory_start
  digitalWrite(utft::RS, HIGH);
  digitalWrite(utft::CS, LOW);
  utft::Address_set(0, 0, 479, 399);

  for (i = 0; i < 480; i++)
    for (m = 0; m < 400; m++)
    {
      utft::Write_Data(j);
    }
  digitalWrite(utft::CS, HIGH);
}

