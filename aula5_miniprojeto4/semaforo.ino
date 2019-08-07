int ledVerde = 2;
int ledAmarelo = 3;
int ledVermelho = 4;

void setup(){
  
  Serial.begin(9600);
}

void loop(){

    digitalWrite(ledVermelho, HIGH);
    Serial.println("Vermelho");
    delay (20000); 
    
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, HIGH);
    Serial.println("Verde");
    delay (30000); 
    
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    Serial.println("Amarelo");
    delay (5000); 
    
    digitalWrite(ledAmarelo, LOW);    
      

}
