
#define OFFSET 0x20

// Macro helper for registers
#define REG(addr) *((volatile unsigned char *)(addr + OFFSET))
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#define time_t unsigned long long

volatile unsigned long delayMilliCounter = 0;
// volatile unsigned const char LEDPIN = 0;
volatile unsigned const char EXTERNALLEDPIN = 1;
volatile unsigned char adcStateVariable = 0; // 0 means reading from PB3, 1 means from PB4

volatile time_t timeCounter = 0;

volatile char stateStatusTank1 = 100; // 0 means no waterflow, 1 means water flow, 2 is error, 100 means need initalize
volatile char previousStateStatusTank1 = 0;
time_t maxFlowTimeTank1 = 4;
// volatile char maxFlowTimeErrorCounterTank1 = 0;
time_t maxNoFlowTimeTank1 = 2;
// volatile char maxNoFlowTimeErrorCounterTank1 = 0;
volatile time_t startFlowTimeTank1;
volatile time_t endFlowRateTimeTank1;

volatile double PIN3AdcReading = 0;

#define BV_MASK(bit) (1 << (bit))
// the trigger function for timer 0
void __vector_10(void) __attribute__((signal));
void __vector_10(void)
{

    TIFR |= ~BV_MASK(4); // clear the interrupt flag
    delayMilliCounter++;
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

    delayMilliCounter = 0;

    while (delayMilliCounter < time)
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
    // run in free running mode

    // enable ADC
    ADCSRA |= _BV(ADIE);

    ADCSRA |= _BV(ADEN); // enable ADC
    ADCSRA |= _BV(ADSC); // Start first conversion
}

time_t readTimeCounter()
{
    return timeCounter;
}
time_t timeDifferenceInMinute(time_t t)
{
    return t / 60;
}
time_t timeDifference(time_t oldTime, time_t newerTime)
{
    return newerTime - oldTime;
}
char detectFlowSensor1()
{
    if ((PINB & BV_MASK(0)) == 0)
    {
        // means no water flow
        return 0;
    }
    else
    {
        return 1;
    }
}
void updateStateStatus(volatile char *previousStatus, volatile char *stateStatus, char newStatus)
{

    *previousStatus = *stateStatus;
    *stateStatus = newStatus;
}

void controlLEDBaseOnStatus(char stateStatus, char previousStateStatus, char errorLEDPin)
{

    // for our strategy, different LED has different delay time period

    // constant water flow: just turn on the LED
    // no water flow: toggled LEd

    // // 1. turn off the led (in sink operation)
    // PORTB |= (1 << errorLEDPin);
    // SHOULD error be erased?
    if (previousStateStatus == 2 || stateStatus == 2)
    {
        // digitalWrite(errorLEDPin, HIGH);
        //  means in error
        if (stateStatus == 0 || previousStateStatus == 0)
        {
            // means no flow present
            // toggle led
            PORTB ^= (1 << errorLEDPin);
            // digitalWrite(noFlowPresentLEDPin, HIGH);
        }
        else
        {
            // means constant water flow
            // just turn on the error led
            PORTB = PORTB & ~(1 << errorLEDPin);
            // digitalWrite(flowPresentLEDPin, HIGH);
        }
    }
    else
    {
        // means it is normal, no red light

        PORTB |= (1 << errorLEDPin); // note is is in sink configure
    }
}

void initializeTankTimerInfo()
{

    stateStatusTank1 = 100;
    previousStateStatusTank1 = 0;
    timeCounter = 0;
    startFlowTimeTank1 = 0;
    endFlowRateTimeTank1 = 0;
}
void readTankSensorLogic(char sensorVal, time_t timeNow, volatile time_t *startFlowTimePtr, volatile time_t *endFlowRateTimePtr,
                         volatile char *stateStatusPtr, volatile char *previousStateStatusPtr,
                         const time_t maxFlowTime,
                         const time_t maxNoFlowTime)
{

    //   char *noFlowMessage, *constantFlowMessage;
    //   if(tankNumber == 1){
    //     //Serial.println("********** Tank 1 ***********");
    //     noFlowMessage = "No water flow in tank 1";
    //     constantFlowMessage = "constant water flow in tank 1";
    //   }else if(tankNumber == 2){
    //     //Serial.println("********** Tank 2 ***********");
    //     noFlowMessage = "No water flow in tank 2";
    //     constantFlowMessage = "constant water flow in tank 2";
    //   }else{
    //     //Serial.println("********** Tank 3 ***********");
    //     noFlowMessage = "No water flow in tank 3";
    //     constantFlowMessage = "constant water flow in tank 3";
    //   }
    ////Serial.print("the curent state is ");
    ////Serial.println(*stateStatusPtr, DEC);
    ////Serial.print("The previous state is ");
    ////Serial.println(*previousStateStatusPtr, DEC);
    ////Serial.print("The current sensor reading is ");
    ////Serial.println(sensorVal, DEC);

    if (*stateStatusPtr == 100)
    {
        // means need initialize
        if (sensorVal == 1)
        {
            *startFlowTimePtr = timeNow;
            updateStateStatus(previousStateStatusPtr, stateStatusPtr, 1);
        }
        else
        {
            *endFlowRateTimePtr = timeNow;
            updateStateStatus(previousStateStatusPtr, stateStatusPtr, 0);
        }
    }
    else if (*stateStatusPtr == 0)
    {
        // means no water flow

        if (sensorVal == 1)
        {
            // means flow  start
            *startFlowTimePtr = timeNow;
            updateStateStatus(previousStateStatusPtr, stateStatusPtr, 1);
            // *maxFlowTimeErrorCounterPtr = 0;
            // *maxNoFlowTimeErrorCounterPtr = 0;
        }
        else
        {
            // means no flow
            time_t difference = timeDifference(*endFlowRateTimePtr, timeNow);
            time_t minute = timeDifferenceInMinute(difference);
            ////Serial.print("The non water flow time difference in second is ");
            ////Serial.print(difference);

            ////Serial.print("The max no flowtime error counter is ");
            ////Serial.println(*maxNoFlowTimeErrorCounterPtr, DEC);

            // int temp = (minute - maxNoFlowTimeTank1 * maxNoFlowTimeErrorCounterTank1);
            // ////Serial.print("The no water flow differece is ");
            // ////Serial.println(temp);
            // if ((minute - maxNoFlowTime * (*maxNoFlowTimeErrorCounterPtr)) >= maxNoFlowTime)
            if (minute >= maxNoFlowTime)
            {
                // need to trigger an error state to send email
                updateStateStatus(previousStateStatusPtr, stateStatusPtr, 2);
            }
            else
            {
                // let us continue waiting
                // this could mean
                // no error happen, maxNoFlowTimeErrorCounterTank1 ==0
                // or error already happened once, and we already send a email about it.
                ;
            }
        }
    }
    else if (*stateStatusPtr == 1)
    {
        if (sensorVal == 1)
        {
            // water is flowing;

            time_t difference = timeDifference(*startFlowTimePtr, timeNow);
            time_t min = timeDifferenceInMinute(difference);
            ////Serial.print("The water flow time difference in second is ");
            ////Serial.println(difference);

            ////Serial.print("The max flowtime error counter is ");
            ////Serial.println(*maxFlowTimeErrorCounterPtr, DEC);
            // int temp = min - maxFlowTimeTank1 * maxFlowTimeErrorCounterTank1;
            // // ////Serial.print("The water flow difference is ");
            // // ////Serial.println(temp, DEC);
            // if ((min - maxFlowTime * (*maxFlowTimeErrorCounterPtr)) >= maxFlowTime)
            if (min >= maxFlowTime)
            {
                // trigger into error state
                updateStateStatus(previousStateStatusPtr, stateStatusPtr, 2);
            }
            else
            {
                // continue waiting
                // means either error alread happened and already send out a email about it,  maxFlowTimeErrorCounterTank1>1, or no error happened yet.
            }
        }
        else
        {
            // means need to switch to state 0
            updateStateStatus(previousStateStatusPtr, stateStatusPtr, 0);
            // *maxFlowTimeErrorCounterPtr = 0;
            // *maxNoFlowTimeErrorCounterPtr = 0;
            *endFlowRateTimePtr = timeNow;
        }
    }
    else
    {
        // do nothing in the error state, since I am not sending any more message to email
        // TODO: maybe do an alarm?

        if (*previousStateStatusPtr == 1)
        {
            //   Serial.print("Error for water is flowing: ");
            //   time_t diff = timeDifference(*startFlowTimePtr, timeNow);
            //   int minutes = timeDifferenceInMinute(diff);
            //   Serial.println(minutes, DEC);
            //   if (*maxFlowTimeErrorCounterPtr == 0) {
            //     bool status = sendEmailAlert(constantFlowMessage);
            //     if (status == false) {
            //       // means gmail did not send through
            //       // do not update status, comeback again
            //       *maxFlowTimeErrorCounterPtr = 0;  // means come back later
            //       *maxNoFlowTimeErrorCounterPtr = 0;
            //       Serial.println("Failed to send out constant waterflow email");

            //     } else {
            //       // means able to send warning email
            //       updateStateStatus(previousStateStatusPtr, stateStatusPtr, 1);
            //       (*maxFlowTimeErrorCounterPtr)++;
            //       (*maxNoFlowTimeErrorCounterPtr) = 0;
            //     }
            //   } else {
            //     // error email already send, do not send email, but update the rest.
            //     updateStateStatus(previousStateStatusPtr, stateStatusPtr, 1);
            //     (*maxFlowTimeErrorCounterPtr)++;
            //     (*maxNoFlowTimeErrorCounterPtr) = 0;
            //   }

            updateStateStatus(previousStateStatusPtr, stateStatusPtr, 1);
        }
        else
        {
            //   Serial.println("Error for no water is flowing");
            //   time_t diff = timeDifference(*endFlowRateTimePtr, timeNow);
            //   int minutes = timeDifferenceInMinute(diff);
            //   Serial.println(minutes);

            //   if (*maxNoFlowTimeErrorCounterPtr == 0) {
            //     bool status = sendEmailAlert(noFlowMessage);
            //     if (status == false) {
            //       // means gmail did not send through
            //       // do not update status, comeback again
            //       *maxFlowTimeErrorCounterPtr = 0;  // means come back later
            //       *maxNoFlowTimeErrorCounterPtr = 0;
            //     } else {
            //       // means able to send warning email
            //       updateStateStatus(previousStateStatusPtr, stateStatusPtr, 0);
            //       *maxFlowTimeErrorCounterPtr = 0;
            //       (*maxNoFlowTimeErrorCounterPtr)++;
            //     }
            //   } else {
            //     // error email already send, do not send email, but update the rest
            //     updateStateStatus(previousStateStatusPtr, stateStatusPtr, 0);
            //     *maxFlowTimeErrorCounterPtr = 0;
            //     (*maxNoFlowTimeErrorCounterPtr)++;
            //   }
            // controlLEDBaseOnStatus(0,2, EXTERNALLEDPIN);
            updateStateStatus(previousStateStatusPtr, stateStatusPtr, 0);
        }
    }

    controlLEDBaseOnStatus(*stateStatusPtr, *previousStateStatusPtr, EXTERNALLEDPIN);
    //   if(tankNumber == 1){
    // controlLEDBaseOnStatus(*stateStatusPtr, *previousStateStatusPtr, EXTERNALLEDPIN);
    //   }else if(tankNumber == 2){
    //     controlLEDBaseOnStatus(stateStatusTank2, previousStateStatusTank2, ERROR_LED_TANK_2, FLOW_PRESENT_TANK_2, NO_FLOW_PRESENT_TANK_2);
    //   }else{
    //     controlLEDBaseOnStatus(stateStatusTank3, previousStateStatusTank3, ERROR_LED_TANK_3, FLOW_PRESENT_TANK_3, NO_FLOW_PRESENT_TANK_3);
    //   }
}

void init()
{
    disableGlobalInterrupt();
    enableIOOutput(EXTERNALLEDPIN);
    setUpTimer();

    setFallTriggerOnPCINTx(2); // set PCIN2 as fall triggered
    enableGlobalInterrupt();
    initializeTankTimerInfo();
}

void updateTimerConstraint(char timerType, double voltageReading)
{

    char timeGain = 1;
    time_t newTimeInMinute = 0;
    if (voltageReading > 4.5)
    {

        newTimeInMinute = 6;
    }
    else if (voltageReading > 3.5)
    {

        newTimeInMinute = 5;
    }
    else if (voltageReading > 2.5)
    {

        newTimeInMinute = 4;
    }
    else if (voltageReading > 1.5)
    {
        newTimeInMinute = 3;
    }
    else if (voltageReading > 0.5)
    {
        newTimeInMinute = 2;
    }
    else
    {
        newTimeInMinute = 1;
    }

    newTimeInMinute *= 1;


    if (timerType == 0)
    {
        // means update the maxNoFlowTime
        maxNoFlowTimeTank1 = newTimeInMinute;
    }
    else
    {
        // means update the maxFlowTime
        maxFlowTimeTank1 = newTimeInMinute;
    }


}
int main()
{
    // SREG |= BV_MASK(7);
    init();
    PORTB |= (1 << EXTERNALLEDPIN); // turn off led
    // IDEAS
    //  use PCIE! to detect inital press
    //  then use software trigger on INT0 to configure and read from 2ADCd
    while (1)
    {

        readTankSensorLogic(detectFlowSensor1(), readTimeCounter(), &startFlowTimeTank1,
                            &endFlowRateTimeTank1, &stateStatusTank1, &previousStateStatusTank1, maxFlowTimeTank1, maxNoFlowTimeTank1);

        // if(detectFlowSensor1() == 0){
        //                 PORTB ^= (1 << EXTERNALLEDPIN);
        // }else{
        //                 PORTB = PORTB & ~(1 << EXTERNALLEDPIN);
        // }
        delayMillisecond(1000);
        timeCounter++;
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
    ADCSRA |= ~BV_MASK(4); // should do it automatically?

    // PORTB ^= (1 << LEDPIN);

    int adc_l = ADCL;                  // value of Input  Voltage in lower register
    int adc_val = (ADCH << 8) | adc_l; // Reading ADCH and coimbining the data
    double voltage = (adc_val * 5.0) / 1024; // select 5 volts as reference
    if (adcStateVariable == 0)
    {
        PIN3AdcReading = voltage;
        adcStateVariable = 1;

        ADCSRA = 0;
        setupADC(4);
    }
    else
    {




        if (voltage <= 2.5)
        {
            updateTimerConstraint(0, PIN3AdcReading);
        }
        else
        {
            updateTimerConstraint(1, PIN3AdcReading);
        }

        // now, reset timer
        initializeTankTimerInfo();

        // PORTB |= BV_MASK(2);
        adcStateVariable = 0;
        ADCSRA = 0; // disable ADC
    }
}