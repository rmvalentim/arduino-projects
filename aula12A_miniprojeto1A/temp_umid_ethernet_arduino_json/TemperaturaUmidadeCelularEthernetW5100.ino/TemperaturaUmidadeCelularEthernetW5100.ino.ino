#include "DHT.h"
#include <SPI.h>
#include <Ethernet.h>

byte mac[]      = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[]       = { 192, 168, 0, 178 }; 
byte gateway[]  = { 192, 168, 0, 1 }; 
byte subnet[]   = { 255, 255, 255, 0 }; 
EthernetServer server(80); //CASO OCORRA PROBLEMAS COM A PORTA 80, UTILIZE OUTRA (EX:8082,8089)


#define DHTPIN A1
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

String readString = String(30);
boolean SENSOR_ON  = false; 

String umidade; 
String temperatura; 
 
void setup() 
{
  Ethernet.begin(mac, ip, gateway, subnet); // INICIALIZA A CONEXÃO ETHERNET
  dht.begin();  
}
 
void loop() 
{
  EthernetClient client = server.available();
  if (client) { // SE EXISTE CLIENTE
    while (client.connected()) { //ENQUANTO  EXISTIR CLIENTE CONECTADO
      if (client.available()) { 
        char c = client.read(); 
        if (readString.length() < 100) 
        {
          readString += c; 
        }
        
        if (c == '\n') { // SE ENCONTRAR "\n" É O FINAL DO CABEÇALHO DA REQUISIÇÃO HTTP
          if (readString.indexOf("?") <0) //SE NÃO ENCONTRAR O CARACTER "?"
          {
          }
          else //SENÃO
            if(readString.indexOf("L=1") > 0){ //SE ENCONTRAR O PARÂMETRO "L=1"
              umidade     = String(dht.readHumidity()); 
              temperatura = String(dht.readTemperature()); 
              SENSOR_ON = true;
            }
          
          client.println("HTTP/1.1 200 OK"); // ESCREVE PARA O CLIENTE A VERSÃO DO HTTP
          client.println("Content-Type: text/html"); // ESCREVE PARA O CLIENTE O TIPO DE CONTEÚDO(texto/html)
          client.println();

          if (SENSOR_ON == true){ 

           String json = "{\"temp\":\"" + umidade + "\",\"umid\":\"" + temperatura + "\"}";
                               
           client.println(json); // RETORNA PARA O CLIENTE O JSON

           json = "";
          }
          else
            client.println("Sem Medicao"); // RETORNA PARA O CLIENTE O VALOR DA UMIDADE
            
          readString="";
          client.stop(); // FINALIZA A REQUISIÇÃO HTTP
        }
      }
    }
  }  
}
