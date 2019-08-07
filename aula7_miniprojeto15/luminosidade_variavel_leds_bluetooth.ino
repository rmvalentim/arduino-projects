int LEDOne = 2;
int LEDTwo = 3;

void setup() {
  Serial.begin(9600);
  
  pinMode(LEDOne, OUTPUT);
  pinMode(LEDTwo, OUTPUT);

}


void loop() {
  
  if(Serial.available() > 0) {
    
    int inByte = Serial.read();
    
    switch(inByte) {
      
      case '0':
      
          
      case '1':
      
           
      case '+':
      
           
      case '-':
      
      
      break;
      
      default:
      break;      
    } 
  }
}
