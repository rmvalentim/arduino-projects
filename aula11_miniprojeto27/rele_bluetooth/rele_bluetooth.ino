#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int porta_rele1 = 7;
int porta_rele2 = 8;


void inverteRele(int rele){
  if(digitalRead(rele) == LOW){
    digitalWrite(rele, HIGH);
  } else {
    digitalWrite(rele, LOW);
  }
} 

void imprimeLcd() {
  lcd.clear();
  
  lcd.setCursor(3, 0);
  digitalRead(porta_rele1) == LOW ? lcd.print("LAMPADA 1 OFF") : lcd.print("LAMPADA 1 ON");
  lcd.setCursor(3, 1);
  digitalRead(porta_rele2) == LOW ? lcd.print("LAMPADA 2 OFF") : lcd.print("LAMPADA 2 ON");  
}

void setup(){
  
  Serial.begin(9600);
  pinMode(porta_rele1, OUTPUT);
  pinMode(porta_rele2, OUTPUT);
  lcd.begin(16,2);
  imprimeLcd();
}

void loop(){
  
  if(Serial.available() > 0){
    int entrada = Serial.read();
    
    switch(entrada) {
      case '1':
      inverteRele(porta_rele1);
      break;
      
      case '2':
      inverteRele(porta_rele2);
      break;    
      
      default:       
      break; 
  
    }
  }
  
  imprimeLcd();
    delay(1000);
}
