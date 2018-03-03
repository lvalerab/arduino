//////////////////////////////////////////
// Sección define
/////////////////////////////////////////
#define PIN_A_MQ7 A5
#define PIN_A_MQ135 A4
#define pDHT 13
#define pinFR A0

//////////////////////////////////////////
// Sección include
/////////////////////////////////////////
#include <Bridge.h>
// #include <BridgeServer.h>
// #include <BridgeClient.h>
#include <dht.h>
#include <Wire.h> //Libreria para el protocolo I2C
#include <Adafruit_BMP085_U.h>

//////////////////////////////////////////
// Sección variables locales
/////////////////////////////////////////
// BridgeServer server;
dht dhtObj;
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

//////////////////////////////////////////
// Sección funciones
/////////////////////////////////////////
String GetJSon(float lum, float temp1, float temp2, float hum, float pres, float ppmCo, float ppmO) {
  String val = "";
  val += "{'lum':";
  val += lum;
  val += ",'temp':[";
  val += temp1;
  val += ",";
  val += temp2;
  val += "],'humedad':";
  val += hum;
  val += ",'presion':";
  val += pres;
  val += ",'ppm':[";
  val += ppmCo;
  val += ",";
  val += ppmO;
  val += "]}";
  return val;
}

//Función para
//Convirtir el valor analógico en voltaje
float GetAnalogVolt(float lect) {
  return lect * (5.0 / 1023.0);
}


///////
//Función para obtener el valor dentro de la curva logaritmica (hay que ajusta los valores a y b, el valor R0 normalmente viene de fábrica, y es el valor de la resistencia
//Obtenido con una lectura base y estandar)
//
float GetConcentracion(float v, float a, float b, float R0) {
  float Rs = 1000 * ((5 - v) / v);
  return a * pow(Rs / R0, b);
}


//Definimos la funcion que nos devuelve el porcentade de luz
int Luminosidad(int puerto) {
  int valor = analogRead(puerto);
  //La función map traduce que el valor mínimo tenga una correspondencia del valor minimo indicado
  //y el máximo con el indicado, siendo los valores que devuelve proporcionales al cambio indicado.
  return map(valor, 0, 1024, 0, 100);
}

//////////////////////////////////////////
// Sección setup y loop
/////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Iniciando....");
  Bridge.begin();
  Serial.println("Iniciado bridge");

  //server.listenOnLocalhost();
  //server.begin();

  //Iniciamos el sensor
  if (!bmp.begin())
  {
    Serial.print("No se ha encontrado el dispositivo, revisar las conexiones o la direccion I2C");
    while (1);
  } else {
    Serial.println("Iniciado chip BMP");
  }  
}

void loop() {
  Serial.println("Fase lectura");
  sensors_event_t event; //Instanciamos el objeto event (evento)
  bmp.getEvent(&event); //Obtenemos los registros del dispositivo
  //Leemos los datos de los sensores, y lo transformamos en JSON
  int v_mq7 = analogRead(PIN_A_MQ7);
  int v_mq135 = analogRead(PIN_A_MQ135);
  dhtObj.read11(pDHT);
  float humedad = dhtObj.humidity;  
  float temperatura = dhtObj.temperature;  
  float lum = Luminosidad(pinFR);
  float temp2=-1;
  float pres=-1;

  
  if (event.pressure) {
    bmp.getTemperature(&temp2); //Le pasamos por referencia la variable, la función sobreescribirá internamente el valor de la variable
    pres = event.pressure;
  } else {
    temp2 = -1;
    pres = -1;
  }
  
  Serial.println("Fase envio datos");
  String json = GetJSon(lum, temperatura, temp2, humedad, pres, GetConcentracion(GetAnalogVolt(v_mq7), 0.4091, -1.497, 5463), GetConcentracion(GetAnalogVolt(v_mq135), 0.4091, -1.497, 5463));
  MySqlSave(lum, temperatura, temp2, humedad, pres,GetConcentracion(GetAnalogVolt(v_mq7), 0.4091, -1.497, 5463), GetConcentracion(GetAnalogVolt(v_mq135), 0.4091, -1.497, 5463));
  Serial.println("Json");
  Serial.println(json);
  /*BridgeClient client = server.accept();
  if (client) {
    client.println(json);
    client.stop();
  }*/
  Serial.println("Fin de la fases, esperando 1 seg");
  delay(1000);
}

void MySqlSave(int lum, int tem0, float tem1, int hum, float pres, float ppmco, float ppmo) {
  Process logdata;

  logdata.begin("lua");
  logdata.addParameter("/root/SetMysqlData.lua");
  logdata.addParameter(String(lum));
  logdata.addParameter(String(tem0));
  logdata.addParameter(String(tem1));
  logdata.addParameter(String(hum));
  logdata.addParameter(String(pres));
  logdata.addParameter(String(ppmco));
  logdata.addParameter(String(ppmo));
  logdata.run();
  while(logdata.available()>0) {
    char c=logdata.read();
    Serial.print(c);
  }
  Serial.println("Fin de envio");
}

