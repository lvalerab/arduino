/*********************************************************************
 * CONSTANTES
 *********************************************************************/
///Definimos que el pin de la fotoresistencia es el A0
#define pinFR A0

/*********************************************************************
 * LIBRERIAS
 *********************************************************************/

/*********************************************************************
 * FUNCIONES
 *********************************************************************/

//Definimos la funcion que nos devuelve el porcentade de luz
int Luminosidad(int puerto) {
  int valor=analogRead(puerto);
  //La función map traduce que el valor mínimo tenga una correspondencia del valor minimo indicado 
  //y el máximo con el indicado, siendo los valores que devuelve proporcionales al cambio indicado.
  return map(valor,0,1024,0,100);
}


/*********************************************************************
 * CONFIGURACION
 *********************************************************************/
void setup() {
  //Abrimos el puerto serial
  Serial.begin(9600);
}


/*********************************************************************
 * BUCLE DE EJECUCION
 *********************************************************************/
void loop() {
  //Enviamos al puerto serial, el valor indicado
  Serial.println(Luminosidad(pinFR));
  //Ponemos un delay para poder leer correctamente los datos
  delay(60000);
}
