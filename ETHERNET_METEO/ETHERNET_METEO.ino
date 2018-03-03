#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

#include <SD.h>

//Constantes para la conexion por red

byte mac[] {
  0xDE, 0xAD, 0xBE, 0xEF, 0xED
};

IPAddress ip(192,168,0,200);
IPAddress gw(192,168,0,1);
IPAddress ms(255,255,255,0);

EthernetServer server(80);

void InicioServer() {
    Serial.begin(9600);
    Ethernet.begin(mac,ip);
    
}

void ConexionServer(String json) {
  EthernetClient client=server.available();
  if(client) {
    Serial.println("Se ha conectado un usuario");
    boolean clib=true; //La linea actual es una linea en blanco
    while(client.connected()) {
      if(client.available()) {
        char c=client.read();
        if(c=='\n' && clib) {
          //Mandamos el json
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println(json);
        }
        if(c=='\n') {
          clib=true;
        } else if (c!='\r') {
          clib=false;
        }
      }
    }
    delay(1);
    client.stop();
  }
}


void setup() {
  // put your setup code here, to run once:
  InicioServer();
}

void loop() {
  // put your main code here, to run repeatedly:
  ConexionServer("<html><head></head><body><p>Estas conectado a arduino</p></body></html>");
}
