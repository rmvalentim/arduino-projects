int Do = 262;
int Re = 294;
int Mi = 330;
int Fa = 349;
int Sol = 392;
int La = 440;
int Si = 523;

int som = 9;

void setup(){
  pinMode(ledDo,OUTPUT);
  pinMode(ledRe,OUTPUT);
  pinMode(ledMi,OUTPUT);
  pinMode(ledFa,OUTPUT);
  pinMode(ledSol,OUTPUT);
  pinMode(ledLa,OUTPUT);
  pinMode(ledSi,OUTPUT);
  
  pinMode(som,OUTPUT);
  
  Serial.begin(9600);
}

void loop(){
  
  if(Serial.available() > 0){
    int entrada = Serial.read();
    
    switch(entrada) {
      case '1':
      tone(9, Do);
      noTone(9);
      delay(100);
      break;
      
      case '2':
      tone(9, Re);
      noTone(9);
      delay(100);
      break;
      
      case '3':
      tone(9, Mi);
      noTone(9);
      delay(100);
      break;
      
      case '4':
      tone(9, Fa);
      noTone(9);
      delay(100);
      break;
      
      case '5':
      tone(9, Sol);
      noTone(9);
      delay(100);
      break;
      
      case '6':
      tone(9, La);
      noTone(9);
      delay(100);
      break;
      
      case '7':
      tone(9, Si);
      noTone(9);
      delay(100);
      break;
      
      default:       
      break; 
  
    }
  }
}
