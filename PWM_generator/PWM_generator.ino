float Tastgrad= 30; // in %
 float fPWM=250;



int main() {

  init();
  Serial.begin(9600);
  
  
  //PIN 9,10 als Ausgänge
  DDRB |= (1 << DDB1)|(1 << DDB2);
  
  //alte Einstellungen löschen
  TCCR1B = 0;
  TCCR1A = 0;
  
  //Einstellen des Prescalers auf 1
  TCCR1B |=  (1 << CS10);
  
  //Modus Einstellen : Pwm Mode 15 mit OCR1A als obere Grenze des Zählers
  TCCR1A |= (1 << WGM11) | (1 << WGM10);
  TCCR1B |= (1 << WGM12) | (1 << WGM13);
  
  //Verhalten des Ausgangs : nicht invertierender Modus für Timer Ausgang b, a ist deaktiviert
  TCCR1A |= (1 << COM1B1);
  
  OCR1A = (16000000./fPWM)-1; //obere Grenze
  OCR1B = (float)((Tastgrad*(OCR1A+1))/100)-1;//Ausgangssignal, hier Messung der Flanken
 
  
  Serial.println("OCR1A: "+(String)OCR1A);
 
  Serial.print("OCR1B: ");
  Serial.println(OCR1B);
  while(1)
  {}
  
  

}
