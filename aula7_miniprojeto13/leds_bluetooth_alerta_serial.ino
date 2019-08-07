int LEDRedPin = 5;
int LEDYellowPin = 4;
int LEDGreenPin = 3;
int LEDWhitePin = 2;

void setup() {
  Serial.begin(9600);
  
  pinMode(LEDRedPin, OUTPUT);
  pinMode(LEDYellowPin, OUTPUT);
  pinMode(LEDGreenPin, OUTPUT);
  pinMode(LEDWhitePin, OUTPUT);
}

// Inverte o Led passado como parametro
void toggleLED(int LEDPin) {
  digitalWrite(LEDPin, !digitalRead(LEDPin));
  if(digitalRead(LEDPin)){
    
  }
}

void loop() {
  
  if(Serial.available() > 0) {
    
    int inByte = Serial.read();
    
    switch(inByte) {
      case 'r':
      toggleLED(LEDRedPin);
      if( digitalRead( LEDRedPin ) ){ Serial.write("LED VERMELHO ACENDEU!\n"); }
      else{ Serial.write("LED VERMELHO APAGOU!\n"); }       
      break;
      
      case 'y':
      toggleLED(LEDYellowPin);
      if( digitalRead( LEDYellowPin ) ){ Serial.write("LED AMARELO ACENDEU!\n"); }
      else{ Serial.write("LED AMARELO APAGOU!\n"); }
      break;
      
      case 'g':
      toggleLED(LEDGreenPin);
      if( digitalRead( LEDGreenPin ) ){ Serial.write("LED VERDE ACENDEU!\n"); }
      else{ Serial.write("LED VERDE APAGOU!\n"); } 
      break;
      
      case 'w':
      toggleLED(LEDWhitePin);
      if( digitalRead( LEDWhitePin ) ){ Serial.write("LED BRANCO ACENDEU!"); }
      else{ Serial.write("LED BRANCO APAGOU!"); } 
      break;
      
      default:
      digitalWrite(LEDRedPin, LOW);
      digitalWrite(LEDYellowPin, LOW);
      digitalWrite(LEDGreenPin, LOW);
      digitalWrite(LEDWhitePin, LOW);  
      break;  
    
    } 
  }
}
