int ledVerde = 2;
int ledAmarelo = 3;
int ledVermelho = 4;
int ledPedestreVerde = 5;
int ledPedestreVermelho = 6;

void setup(){  
  Serial.begin(9600);
}

void loop(){

    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledPedestreVerde, HIGH);
    Serial.println("Carro: Vermelho - Pedestre: Verde");
    delay (20000); 
    
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledPedestreVerde, LOW);
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledPedestreVermelho, HIGH);
    Serial.println("Carro: Verde - Pesestre: Vermelho");
    delay (30000); 
    
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    Serial.println("Carro: Amarelo - Pesestre: Vermelho");
    delay (5000); 
    
    digitalWrite(ledAmarelo, LOW); 
    digitalWrite(ledPedestreVermelho, LOW);  
}
