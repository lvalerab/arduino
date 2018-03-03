int PR = 13;
int PG = 11;
int PB = 10;
int encendido = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(PR, OUTPUT);
  pinMode(PG, OUTPUT);
  pinMode(PB, OUTPUT);
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void Encender(int color, int intensidad) {
  switch (color) {
    case 0:
      Serial.println("Todos a 0");
      analogWrite(PR, 0);
      analogWrite(PG, 0);
      analogWrite(PB, 0);
      break;
    case 1:
      Serial.print("Rojo a ");
      Serial.println(intensidad);
      analogWrite(PR, intensidad);
      break;
    case 2:
      Serial.print("Verde a ");
      Serial.println(intensidad);
      analogWrite(PG, intensidad);
      break;
    case 3:
      Serial.print("Azul a ");
      Serial.println(intensidad);
      analogWrite(PB, intensidad);
      break;
    case 4:
      Serial.println("Todos a 254");
      analogWrite(PR, 254);
      analogWrite(PG, 254);
      analogWrite(PB, 254);
      break;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  /*Encender(encendido);
    encendido++;
    encendido=(encendido>3?0:encendido);
    delay(1000);*/
  Encender(4, 0);
  delay(1000);
  Encender(0, 0);
  delay(1000);
  for (int l = 1; l < 4; l++) {
    Encender(0, 0);
    for (int r = 0; r < 254; r++) {      
          Encender(l, r);
          delay(75);      
    }
  }

  for(int r=0;r<254;r++) {
    int c=random(0,3);
    delay(200);
    int i=random(0,254);
    delay(200);
    Encender(c,i);
  }

  Encender(4, 0);
  delay(1000);
  Encender(0, 0);
}


