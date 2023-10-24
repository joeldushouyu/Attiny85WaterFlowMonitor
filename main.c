
#define OFFSET 0x20

// Macro helper for registers
#define REG(addr) *((volatile unsigned char *)(addr + OFFSET))
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>
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
volatile unsigned char adcStateVariable = 0; //0 means reading from PB3, 1 means from PB4

#define BV_MASK(bit) (1 << (bit))
// the trigger function for timer 0
void __vector_10(void) __attribute__((signal));
void __vector_10(void)
{

    TIFR |= ~BV_MASK(4); // clear the interrupt flag
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

    // DDRB |= (1 << 0);
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

void setupADC(char pin)
{
    if (pin == 3)
    {
        ADMUX = 0x3;
    }
    else
    {
        // pin is 4
        ADMUX = 0x2;
    }
    // // choose source
    // ADCSRB |= BV_MASK(2) | BV_MASK(1);

    // enable ADC
    ADCSRA |= _BV(ADIE);

        ADCSRA |= _BV(ADEN);  // enable ADC
        ADCSRA |= _BV(ADSC); // Start first conversion
    // do not start conversion right now
}
void init()
{
    disableGlobalInterrupt();
    enableIOOutput(LEDPIN);
    enableIOOutput(EXTERNALLEDPIN);
    setUpTimer();

    setFallTriggerOnPCINTx(2); // set PCIN2 as fall triggered
    setupADC(3);
    enableGlobalInterrupt();
}
int main()
{
    // SREG |= BV_MASK(7);
    init();
    // PORTB |=   (1<< LEDPIN);
    // IDEAS
    //  use PCIE! to detect inital press
    //  then use software trigger on INT0 to configure and read from 2ADCd
    while (1)
    {

        // if (adcStateVariable == 0)
        // {
        //     adcStateVariable = 1;
        //     setupADC(4);
        //     // PORTB |= ~BV_MASK(2);
        // }
        // else
        // {
        //     // PORTB |= BV_MASK(2);
        //     adcStateVariable = 0;
        //     setupADC(3);
        // }

        delayMillisecond(1000);

        // PORTB ^=   (1 << LEDPIN);
    }

    // while(1){

    //     // disable interrupt

    //     // enable interrupt
    // }

    return 0;
}

ISR(PCINT0_vect)
{

    // disable futher interrupt
    //  GIMSK |= ~BV_MASK(5); // use the PCIE option for all

    // clear the interrupt flag
    GIFR |= ~BV_MASK(5);

    // refer to page 12, user allow to do nested interrupt
    enableGlobalInterrupt();
    delayMillisecond(10);
    disableGlobalInterrupt();

    // because PCINT2 is configure as fall, so need to see if still is zero

    char flag = 0;
    while ((PINB & (BV_MASK(2))) == 0)
    {
        ; // wait for until a release
        flag = 1;
    }
    if (flag)
    {
        setupADC(3);

        // PORTB ^= (1 << EXTERNALLEDPIN);
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

ISR(ADC_vect)
{

    // clear interrupt flag
    ADCSRA|= ~BV_MASK(4); // should do it automatically?

    // PORTB ^= (1 << LEDPIN);

    int adc_l = ADCL;                        // value of Input  Voltage in lower register
    int adc_val = (ADCH << 8) | adc_l;       // Reading ADCH and coimbining the data
    double voltage = (adc_val * 5.0) / 1024; // select 5 volts as reference
    if (voltage > 4.0)
    {

        if (adcStateVariable == 0)
        {
            PORTB = PORTB & ~(1<<LEDPIN);
        }
        else
        {
            PORTB =PORTB &~(1<<EXTERNALLEDPIN);
        }
        // PORTB |=   (0 << LEDPIN);
    }else{
        if (adcStateVariable == 0)
        {
            PORTB |= (1 << LEDPIN);
        }
        else
        {
            PORTB |= (1 << EXTERNALLEDPIN);
        }
    }

    if (adcStateVariable == 0)
    {
        adcStateVariable = 1;

        ADCSRA = 0;   
        setupADC(4);


    }
    else
    {
        // PORTB |= BV_MASK(2);
        adcStateVariable = 0;
        ADCSRA = 0;   // disable ADC

    }
}