
#define OFFSET 0x20

// Macro helper for registers
#define REG(addr) *((volatile unsigned char *)(addr + OFFSET))
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
// #define SREG  REG(0x1F)

// #define DDRB REG(0x17)
// #define PORTB REG(0x18)

// #define CLKPR REG(0x26)
// //timer
// #define GTCCR REG(0x2C)
// #define TCCR0A REG(0x2A)
// #define TCCR0B REG(0x33)
// #define TCNT0 REG(0x32)
// #define OCR0A REG(0x29)
// #define TIMSK REG(0x39)
// #define TIFR REG(0x38)

volatile unsigned long count = 0;
volatile unsigned const char LEDPIN = 0;
volatile unsigned const char EXTERNALLEDPIN = 1;

#define BV_MASK(bit) (1 << (bit))
// the trigger function for timer 0
void __vector_10(void) __attribute__((signal));
void __vector_10(void) {
    
    TIFR |= ~BV_MASK(4); //clear the interrupt flag
     count++; 
     
     
}

void disableGlobalInterrupt()
{

    SREG |= ~BV_MASK(7); // disable all interrupt
}
void enableGlobalInterrupt()
{

    SREG |= BV_MASK(7); // disable all interrupt
}
void delayMillisecond(unsigned long time)
{

    count = 0;

    while (count < time)
    {
    }
}

// use set OCR0A to specific interrupt time(with prescaler of 8)

void setUpTimer()
{

    //DDRB |= (1 << 0);
    GTCCR = BV_MASK(7);
    TCCR0A = 0; // TODO:
    TCCR0B = 0;
    TCCR0A |= BV_MASK(1); // TODO:
    TCCR0B |= BV_MASK(1);
    TCNT0 = 0x0;  // start counting from 0
    OCR0A = 0x7D; // 1MHZ => 1000 times in 1 ms, if scale by 8, means need
    TIMSK |= BV_MASK(4);
    ; // enable interrupt?

    // setup
    // 1. write TSM to activate the counter

    // GTCCR = 0x00;
}

void setFallTriggerOnINT0()
{

    MCUCR |= (BV_MASK(1) | ~BV_MASK(0));
    GIMSK |= BV_MASK(6); // use the PCIE option for all
}
void setFallTriggerOnPCINTx(char x)
{
    MCUCR |= (BV_MASK(1) | ~BV_MASK(0));

    // GIFR(5) flag will raise
    PCMSK |= BV_MASK(x); // pin 2,
    GIMSK |= BV_MASK(5); // use the PCIE option for all
}
void enableIOOutput(char x)
{
    DDRB |= (1 << x); // enable I/o x for output
}
void enableIOInputTriState(char x)
{

    // do nothing, by default is already
    ;
}
void init()
{
    disableGlobalInterrupt();
    enableIOOutput(LEDPIN);
    enableIOOutput(EXTERNALLEDPIN);
    setUpTimer();
    
    enableIOInputTriState(2);
    setFallTriggerOnPCINTx(2); // set PCIN2 as fall triggered
    enableGlobalInterrupt();
}
int main()
{
    // SREG |= BV_MASK(7);
    init();

    // IDEAS
    //  use PCIE! to detect inital press
    //  then use software trigger on INT0 to configure and read from 2ADCd
    while (1)
    {

        delayMillisecond(1000);
   
        PORTB ^=   (1 << LEDPIN);
        // PORTB ^=   (1 << EXTERNALLEDPIN);
        // PORTB = 0x0;
  
    }

    // while(1){

    //     // disable interrupt

    //     // enable interrupt
    // }

    return 0;
}

ISR(PCINT0_vect)
{   

    //disable futher interrupt
    // GIMSK |= ~BV_MASK(5); // use the PCIE option for all

    // clear the interrupt flag
    GIFR |= ~BV_MASK(5);

    // refer to page 12, user allow to do nested interrupt
    enableGlobalInterrupt();
    delayMillisecond(10);
    disableGlobalInterrupt();

    // because PCINT2 is configure as fall, so need to see if still is zero

    char flag = 0;
    while( (PINB & (BV_MASK(2))) == 0){
        ; //wait for until a release
        flag = 1;
    }
    if(flag){
            PORTB ^= (1 << EXTERNALLEDPIN);
    }
            // PORTB ^= (1 << EXTERNALLEDPIN) ;


    // if(v == 0){
    //      PORTB ^= (1 << EXTERNALLEDPIN);
    // }else{
    //     // means probably is noise 
    // }



    // GIMSK |= BV_MASK(5); // use the PCIE option for all
    // setUpTimer();

}