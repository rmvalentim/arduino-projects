int LEDRedPin = 5;
int LEDYellowPin = 4;
int LEDGreenPin = 3;
int LEDWhitePin = 2;

int LEDTarget = 5;

void setup() {
  Serial.begin(9600);
  
  pinMode(LEDRedPin, OUTPUT);
  pinMode(LEDYellowPin, OUTPUT);
  pinMode(LEDGreenPin, OUTPUT);
  pinMode(LEDWhitePin, OUTPUT);
}


void loop() {
  
  if(Serial.available() > 0) {
    
    int inByte = Serial.read();
    
    switch(inByte) {
      case 'r':
      LEDTarget = LEDRedPin;  
      break;
      
      case 'y':
      LEDTarget = LEDYellowPin;
      break;
      
      case 'g':
      LEDTarget  = LEDGreenPin;
      break;
      
      case 'w':
      LEDTarget = LEDWhitePin; 
      break;  
    
      default:        
      int range = map(inByte, 0, 10, 0, 255); 
      analogWrite(LEDTarget, range);
      inByte = 0;
      break;  
    
    } 
  }
}
