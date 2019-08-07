const int sensorMin = 0;
const int sensorMax = 800;
const int pinYellow = 2;
const int pinRed = 3;
const int pinWhite = 4;

int photocellPin = A0;

void setup(){
  
  pinMode(pinYellow,OUTPUT);
  pinMode(pinRed,OUTPUT);
  pinMode(pinWhite,OUTPUT);
}

void loop(){

  int range;
  int analogValue;
  
  analogValue = analogRead(photocellPin);

  range = map(analogValue, sensorMin, sensorMax, 0, 2);
  
  switch(range) {
    case 0:
    digitalWrite(pinYellow,HIGH);
    digitalWrite(pinRed,LOW);
    digitalWrite(pinWhite,LOW);
    break;
  
    case 1:
    digitalWrite(pinYellow,LOW);
    digitalWrite(pinRed,HIGH);
    digitalWrite(pinWhite,LOW);
    break;  
  
    case 2:
    digitalWrite(pinYellow,LOW);
    digitalWrite(pinRed,LOW);
    digitalWrite(pinWhite,HIGH);
    break;         
    
  }
  
  delay(1);      

}
