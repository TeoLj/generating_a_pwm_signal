uint16_t ZaehlerstandfuerT_high, Zaehlimpulsabstand;
unsigned long starttime = 0;
volatile int flag;
//für Berechnungen notwendig
volatile uint16_t Steigende_Flanke1, Fallende_Flanke1, Steigende_Flanke2;

int main() {

  init();
  Serial.begin(9600);
  DDRB &= ~(1 << DDB0);
  //Timer Einstellungen löschen
  TCCR1A = 0;
  TCCR1B = 0;

  //Einstellen des Prescalers => 1 , Prescaler des Generators und Ausleser gleich, damit Verzicht auf Interrupt Overflow
  TCCR1B |=  (1 << CS10);

  //Noise Canceler an
  TCCR1B |= (1 << ICNC1);

  //I.Capture auf steigende Flanke
  TCCR1B |= (1 << ICES1);

  //Modus Einstellen : Normaler Modus
  //Verhalten des Ausgangs : deaktivieren
  //I.Capture Interrupt an
  TIMSK1 |= (1 << ICIE1);

  //Globales Freischalten der Interrupts
  sei();

  while (1) {

    if (flag==3)
    {

      Zaehlimpulsabstand = Steigende_Flanke2 - Steigende_Flanke1;//Periodendauer
      ZaehlerstandfuerT_high =  (Fallende_Flanke1 - Steigende_Flanke1) ; //Impulsdauer


      //Berechnung der Frequenz aus der Differenz der Zählerstände zwischen zwei steigenden Flanken (->Zaehlimpulsabstand)
      float Frequenz = 16000000. / (Zaehlimpulsabstand); //Frequenz der einzelenen Zählimpulse

      // Berechnung des Tastgrades in Prozent
      float Tastgrad =  100.* ZaehlerstandfuerT_high / Zaehlimpulsabstand; // Anzeit/Periodendauer


      while ((millis() - starttime) > 1000)
      {
        Serial.print(" Frequenz : ");
        Serial.print(Frequenz);
        Serial.print(" Hz || ");
        Serial.print(" Tastgrad : ");
        Serial.print(Tastgrad);
        Serial.println(" %");
        starttime=millis();
      }
      flag=0;
      sei();
    }
  }
}


  ISR(TIMER1_CAPT_vect)
  {

    switch (flag)
    {
      case 0:
        Steigende_Flanke1= ICR1;
        TCCR1B &= ~(1 << ICES1); //auf fallende Flanke triggern
       
        break;

      case 1:
        Fallende_Flanke1 = ICR1;
        TCCR1B |= (1 << ICES1);
        
        break;

      case 2:
        Steigende_Flanke2 = ICR1;    
        cli(); //keine fortlaufende Messung
        
        break;
    }
    flag++;
}
