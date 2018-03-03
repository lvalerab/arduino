



//////////////////////////////////////////
// Sección define
/////////////////////////////////////////

//////////////////////////////////////////
// LIBRERIAS
/////////////////////////////////////////
#include <Wire.h> //Libreria para el protocolo I2C
#include <Adafruit_BMP085_U.h>


//Instanciamos el objeto, indicandole la dirección del chip en el bus I2C (por fabricante)
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
void setup() {
  //Iniciamos el puerto serie para la consola
  Serial.begin(9600);

  //Iniciamos el sensor
  if(!bmp.begin())
  {
    Serial.print("No se ha encontrado el dispositivo, revisar las conexiones o la direccion I2C");
    while(1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  sensors_event_t event; //Instanciamos el objeto event (evento)
  bmp.getEvent(&event); //Obtenemos los registros del dispositivo

  if(event.pressure) {
    float temp;
    //Serial.print("Temperatura: ");
    bmp.getTemperature(&temp); //Le pasamos por referencia la variable, la función sobreescribirá internamente el valor de la variable
    //Serial.println(temp);
    //Serial.println (" grados centigrados");
    //Serial.print("Presion: ");
    Serial.println(event.pressure);
    //Serial.println(" hPa"); //lo medimos en hectopascales
  } else {
    Serial.println("No hay evento, error de lectura, compruebe el bus I2C o la direccion del dispositivo");
  }
  delay(1000); //Esperamos un segundo a la siguiente lectura

}
