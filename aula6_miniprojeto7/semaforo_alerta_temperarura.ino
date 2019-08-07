#include "DHT.h"

#define DHTPIN A1
#define DHTTYPE DHT11

int ledVerde = 2; 
int ledAmarelo = 3;
int ledVermelho = 4;
 

DHT dht (DHTPIN, DHTTYPE);

void setup() {
        pinMode(ledVerde, OUTPUT);
        pinMode(ledAmarelo, OUTPUT);
        pinMode(ledVermelho, OUTPUT);
	Serial.begin(9600);
	delay(1000);
	dht.begin();
}

void loop() {
  
	float t = dht.readTemperature();

	if(isnan(t)) {
  
		Serial.println("Falha na leitura do DHT");

	} else {
  
		if(t <= 30){
                    Serial.print("Temperatura: ");
                    Serial.print(t);
                    Serial.println(" C / Nivel Normal");
                    
                } else if (t <= 35){
                    Serial.print("Temperatura: ");
                    Serial.print(t);
                    Serial.println(" C / Nivel de Alerta");
                } else {
                    Serial.print("Temperatura: ");
                    Serial.print(t);
                    Serial.println(" C / Nivel Critico");
                }
                
               
	}

  delay(3000);
	
}
