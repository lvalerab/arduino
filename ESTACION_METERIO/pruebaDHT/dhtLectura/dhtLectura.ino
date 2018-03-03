//////////////////////////////////////////
// Sección define
/////////////////////////////////////////

//Definimos el puerto del
#define pDHT 13

//////////////////////////////////////////
// LIBRERIAS
/////////////////////////////////////////

//Incluimos las librerias de DHT
#include <dht.h>

//Instanciamos un objeto DHT

dht dhtObj;

void setup() {
  //Iniciamos el puerto serie (USB) para poder leer los datos desde el ordenador
  Serial.begin(9600);
}

void loop() {
  //Leemos los datos del pin de datos del DHT (read11 (11 en numero :-D, no es LL)
  dhtObj.read11(pDHT);
  // Impimimos los datos leidos (nos lo proporciona los metodos de la clase DHT
  float humedad=dhtObj.humidity;
  float temperatura=dhtObj.temperature;
  //Las especificaciones de la libreria nos dicen de esperar un minimo de tiempo, 
  //leemos los datos cada minuto.
  delay(1000);
  //Sacamos los datos por serial
  //Serial.print("Temp: ");
  //Serial.print(temperatura);
  //Serial.println(" Grados centígrados");
  //Serial.print("Humedad: ");
  //Serial.print(humedad);
  //Serial.println(" %");
  //Serial.println("=======================================");
  //Serial.println(temperatura);
  Serial.println(humedad);
}
