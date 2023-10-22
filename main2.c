const byte led4 = 4;
const byte led3 = 3;

volatile int count = 0; 

void setup(){
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  cli();
  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |=(1<<WGM01); //Start timer 1 in CTC mode Table 11.5
  TIMSK |= (1 << OCIE0A); //Enable CTC interrupt see 13.3.6
  OCR0A=127; //CTC Compare value
  TCCR0B |= (1 << CS02) | (1 << CS00); // Prescaler =1024 Table 11.6
  sei();
}

ISR(TIMER0_COMPA_vect)
{
  count++;
  if (count>30){
    digitalWrite(led4, ! digitalRead(led4));
    count=0;  
  }

}

void loop(){
  digitalWrite(led3,HIGH);
  delay(1000);
  digitalWrite(led3,LOW);
  delay(1000);
}