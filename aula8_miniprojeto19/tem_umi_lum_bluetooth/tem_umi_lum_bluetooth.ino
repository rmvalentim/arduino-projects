#include "DHT.h"

#define DHTPIN A5
#define DHTTYPE DHT11

DHT dht (DHTPIN, DHTTYPE);

const int sensorMin = 0;
const int sensorMax = 800;
int photocellPin = A0;

float t = 0;
float u = 0;
float l = 0;

void setup() {
	Serial.begin(9600);
	delay(1000);	
	dht.begin();
}

void loop() {
  
        if( Serial.available() > 0) {
        
          char inByte = Serial.read();
          
          switch(inByte){
          
            case 't':
            t = dht.readTemperature();
            if(isnan(t)) {
  
		Serial.println("Falha na leitura do DHT");

	    } else {                 
                Serial.println(t);            
            }
            break;
            
            case 'u':
            u = dht.readHumidity();            
            if(isnan(u)) {
  
		Serial.println("Falha na leitura do DHT");

	    } else {                 
                Serial.println(u);
            }
            break;
            
            case 'l':
              int range;
              int analogValue;
              
              analogValue = analogRead(photocellPin);
            
              range = map(analogValue, sensorMin, sensorMax, 0, 3);
              
              switch(range) {
                case 0:
                Serial.println("Escuro");
                break;
              
                case 1:
                Serial.println("Penumbra");
                break;
                
                case 2:
                Serial.println("Medio");
                break;
                
                case 3:
                Serial.println("Claro");
                break;
                
              }
              
              delay(250);
            break;
            
            default:
            Serial.print("Sem leitura");
            break;
          
          
          }
        
        
        }  

        delay(3000);
	
}
