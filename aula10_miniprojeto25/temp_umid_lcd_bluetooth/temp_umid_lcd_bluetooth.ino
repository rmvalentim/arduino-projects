#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#include "DHT.h"
#define DHTPIN A5
#define DHTTYPE DHT11
DHT dht (DHTPIN, DHTTYPE);

const int sensorMin = 0;
const int sensorMax = 800;
int photocellPin = A0;

float v = 0;
String leitura = "";

void imprimeLcd(String leitura) {
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print(leitura);
  
}

void setup(){  
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16,2);
}

void loop(){
  leitura = "";
  if(Serial.available() > 0){
    int entrada = Serial.read();
    
    switch(entrada) {
      case 't':
        v = dht.readTemperature();
        if(!isnan(v)) { 
            leitura += "Temp: ";
            leitura += v;
            leitura += " C";
      imprimeLcd(leitura);
      }               
      break;
      
      case 'u':
        v = dht.readHumidity();
        if(!isnan(v)) {
            leitura += "Umid: ";
            leitura += v;
            leitura += " %";  
      imprimeLcd(leitura);
      }
      break; 

      case 'l':
           int range;
           int analogValue;
           
           analogValue = analogRead(photocellPin);
                     
           range = map(analogValue, sensorMin, sensorMax, 0, 3);
              
           switch(range) {
             case 0:           
             imprimeLcd("Escuro");
             break;
              
             case 1:
             imprimeLcd("Penumbra");
             break;
                
             case 2:
             imprimeLcd("Medio");
             break;
                
             case 3:
             imprimeLcd("Claro");
             break;

             default:   
             break;
          }  
          
      default:       
      break;   
    }
  }
    delay(1000);
}
