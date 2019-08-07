int ledPin = 2;
int sensorPin = A0;
int param = 100;

int leitura = 0;
int led = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  
}

void loop() {
  
  leitura = analogRead(sensorPin);
  
  if (leitura >= param) {
  
    led = !led;
    
    digitalWrite(ledPin, led);
        
  }
  delay(100);
}
