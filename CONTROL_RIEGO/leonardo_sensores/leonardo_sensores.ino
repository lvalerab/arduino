#include <dht.h>


/* 
Configuracion de pines
*/
#define pSonarPing 12
#define pSonarData 13
#define pDhtData A0
#define pSensorHSuelo A2
#define pSensorNivel A1

#define pBomba1 2
#define pBomba2 3
#define pElecValvula1 4
#define pElecValvula2 5

dht DHT;

/////////////////////////////////////////////////////////////////////
//Inicializar actuadores valvulas
/////////////////////////////////////////////////////////////////////
bool Llenado;
bool Vaciado;
int cuenta=0;

void InicializarActuadores() {
  pinMode(pBomba1,OUTPUT);
  pinMode(pBomba2,OUTPUT);
  pinMode(pElecValvula1,OUTPUT);
  pinMode(pElecValvula2,OUTPUT);
  Llenado=false;
  Vaciado=false;
}

void Bomba(int cual, bool activar) {
  int pin=0;
  switch(cual) {
    case 1:
      pin=pBomba1;
      break;
    case 2:
      pin=pBomba2;
      break;
   }
   if(activar) {
     digitalWrite(pin,HIGH);
     Vaciado=true;
   } else {
     digitalWrite(pin,LOW);
     Vaciado=false;
   }
}

void ElectroValvula(int cual, bool activar) {
  int pin=0;
  switch(cual) {
    case 1:
      pin=pElecValvula1;
      break;
    case 2:
      pin=pElecValvula2;
      break;
   }
   if(activar) {
     digitalWrite(pin,HIGH);
     Llenado=true;
   } else {
     digitalWrite(pin,LOW);
     Llenado=false;
   }
}

//////////////////////////////////////////////////////////////////////
// Parte para el sonar (NIVEL);
//////////////////////////////////////////////////////////////////////

void InicializarSonar() {
  //Establecemos el modo de cada ping
  pinMode(pSonarPing,OUTPUT);
  pinMode(pSonarData,INPUT);
}

long PulsoSonar() {
  long duracion;
  digitalWrite(pSonarPing,LOW);
  delayMicroseconds(2);
  //Mandamos el pulso y esperamos
  digitalWrite(pSonarPing,HIGH);
  delayMicroseconds(10);
  digitalWrite(pSonarPing,LOW);
  duracion=pulseIn(pSonarData,HIGH);
  return duracion;
}

long Distancia(long tiempoPulso) {
    return (tiempoPulso/2)/29.1;
}

float ObtenVs(float T) {
  //return sqrt(1.4*287*(T+273))*(100/3600000);
  return (331.4+(0.61*T))*0.1;
}

long DistanciaT(long tiempoPulso, float Temperatura) {
   float vs=ObtenVs(Temperatura);
  return (tiempoPulso/2)/vs; 
}

void ImprimeNivel() {
  long tiempo=PulsoSonar();
  long distancia=Distancia(tiempo);
  Serial.print("Tiempo: ");
  Serial.print(tiempo);
  Serial.print(" s, Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
}

long NivelCuba() {
  long tiempo=PulsoSonar();
  return Distancia(tiempo);
}
///////////////////////////////////////////////////////
// Sensor Humedad del suelo
///////////////////////////////////////////////////////

void InicializarHumedadSuelo() {
  //pinMode(pSensorHSuelo,INPUT);
}

float LeerPorcentajeHumedadSuelo() {
  long s=analogRead(pSensorHSuelo);
  return s;
}

void ImprimeHumedadSuelo() {
  Serial.print("Humedad suelo: ");
  Serial.print(LeerPorcentajeHumedadSuelo());
  Serial.println(" %");
}

///////////////////////////////////////////////////////
// Sensor Nivel agua
///////////////////////////////////////////////////////

void InicializarNivelAgua() {
  //pinMode(pSensorNivel,INPUT);
}

float LeeNivelAgua() {
  long val=analogRead(pSensorNivel);
  return val;
}

void ImprimeNivelAgua() {
  Serial.print("Nivel agua (sensor directo): ");
  Serial.println(LeeNivelAgua());
}

///////////////////////////////////////////////////////
//Sensor temperatura y humedad ambiente
///////////////////////////////////////////////////////

void InitTH() {
  DHT.read11(pDhtData);
}

float Temperatura() {
  return DHT.temperature;
}

float Humedad() {
  return DHT.humidity;
}

void ImprimeTH() {
  InitTH();
  Serial.print("Humedad ambiente: ");
  Serial.print(Humedad());
  Serial.print(" %, Temperatura: ");
  Serial.print(Temperatura());
  Serial.println(" ºC");
}

String obtenValoresTH() {
  InitTH();
  return String(Temperatura())+"#"+String(Humedad());
} 

///Monta la cadena para transmitir al puerto serie
String cadenaSerie() {
  return obtenValoresTH()+"#"+String(LeerPorcentajeHumedadSuelo())+"#0#"+String(NivelCuba());
}

void setup() {
  //Abrimos el puerto serie
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("Configurando sensores");
  InicializarSonar();
  InicializarHumedadSuelo();
  InicializarNivelAgua(); 
  InicializarActuadores();
  delay(1200);  
  Serial.println("Iniciando lectura sensores");
}





void loop() {
  // put your main code here, to run repeatedly:
//  ImprimeNivel();
//  ImprimeHumedadSuelo();
//  ImprimeNivelAgua();
//  ImprimeTH();
//  Serial.print("cuenta: ");
//  Serial.println(cuenta);
  if(cuenta==10) {
    Bomba(1,true);
    Bomba(2,true);
  }
  cuenta++;
  String cdsr1=cadenaSerie();
  Serial1.println(cdsr1);
  Serial.println(cdsr1);
  delay(1000);
  if(cuenta==20) {
    Bomba(1,false);
    Bomba(2,false);
    cuenta=0;
   }
}
