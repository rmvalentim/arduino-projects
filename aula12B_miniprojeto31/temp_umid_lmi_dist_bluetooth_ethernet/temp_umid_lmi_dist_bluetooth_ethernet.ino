#include <Ultrasonic.h>
#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"

// DHT 11
#define DHTPIN A0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LDR
int photocellPin = A1;
const int sensorMin = 0;
const int sensorMax = 800;

// Ultrasonic
#define PINO_TRIGGER  4 
#define PINO_ECHO     5 
Ultrasonic ultrasonic(PINO_TRIGGER, PINO_ECHO);
 
//Definicoes de IP, mascara de rede e gateway
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192,168,0,150);            //Define o endereco IP
IPAddress gateway(192,168,0,1);        //Define o gateway
IPAddress subnet(255, 255, 255, 0);    //Define a máscara de rede 
EthernetServer server(80);
 
void setup(){
  
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  dht.begin();
  Serial.begin(9600);
}
 
void loop() {
  
  float cmMsec, polMsec;
  String temp, umidade;
  long microsec = ultrasonic.timing();
  int range, analogValue;
  
  //Le e armazena as informacoes do sensor ultrasonico
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  polMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
 
  //Aguarda conexao do browser
  EthernetClient client = server.available();
  if (client) {
    // Um Request HTTP termina com uma linha em branco
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        //Se recebeu uma nova linha e a linha eh branca, a HTTP Request eh finalizada
        //Então pode enviar um reply 
        if (c == 'n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>"); 

          if(Serial.available() > 0){
            int entrada = Serial.read();
                        
            switch(entrada) {
              // Temperatura
              case '1':            
              temp = String(dht.readTemperature()); 
               client.print("Temperatura: ");
               client.print("<b>");
               client.print(temp);                
               client.println("</b></html>"); 
              break;
              // Umidade
              case '2':
               umidade = String(dht.readHumidity()); 
               client.print("Umidade: ");
               client.print("<b>");
               client.print(umidade);                
               client.println("</b></html>"); 
              break;
              // Luminosidade
              case '3':                 
                analogValue = analogRead(photocellPin); 
                range = map(analogValue, sensorMin, sensorMax, 0, 3);
              
                client.print("Luminosidade: ");
                client.print("<b>");
                switch(range) {
                  case 0:
                  client.print("Escuro");
                  break;                
                  case 1:
                  client.print("Penumbra");
                  break;                  
                  case 2:
                  client.print("Medio");
                  break;                  
                  case 3:
                  client.print("Claro");
                  break;                
                }               
                client.println("</b></html>");
              break;
              // Distancia Cm
              case '4':                
                client.print("Sensor Ultrasonico Cm: ");
                client.print("<b>");
                client.print(cmMsec);
                client.print(" cm");
                client.println("</b></html>");
              break;
              // Distancia Pol
              case '5':
                client.print("Sensor Ultrasonico Pol: ");
                client.print("<b>");
                client.print(polMsec);
                client.print(" polegadas");
                client.println("</b></html>");
              break;
              
              default:  
              client.println("<span>Sem medicao</span>");     
              break; 
          
            }
          } else {
              client.println("<span>Sem medicao</span>");
              
            } 
        }
        
        if (c == 'n') {
          // Inicia uma nova linha
          currentLineIsBlank = true;
        } 
        else if (c != 'r') {
          // Recebeu um caracter na nova linha
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    client.stop();
  }
}
