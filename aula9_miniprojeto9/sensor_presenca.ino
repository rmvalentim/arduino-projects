int TRIGGER = 4;
int ECHO = 5;
int sirene = 9;
float seno, distance; 
int frequencia, timeMicro;

void setup() {
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
}

void tocarSirene() {
	for(int x = 0; x < 180; x++){
		seno = (sin(x*3.1416/180));		
		frequencia = 2000+(int(seno*1000));
		tone(sirene, frequencia);
		delay(2);
	}	
}

void loop() {

  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(20);  
  digitalWrite(TRIGGER, HIGH);  
  delayMicroseconds(100);  
  digitalWrite(TRIGGER, LOW);  
  timeMicro = pulseIn(ECHO, HIGH);  
  distance = timeMicro / 29.387 / 2;  
  
   if(distance < 50 ) { tocarSirene(); 
   } else { noTone(sirene);
   }
  
  delay(1000);
}
