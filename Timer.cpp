#if defined(ATSAMD21G)

#include "ATSAMD21G/Timer.h"

Timer TimerA(TCC0);
Timer TimerB(TCC1);
Timer TimerC(TCC2);

void TCC0_Handler() {
    if(TCC0->INTFLAG.bit.OVF) {
        TCC0->INTFLAG.bit.OVF = 1;
        TimerA.isrCallback();
    }
}

void TCC1_Handler() {
    if(TCC1->INTFLAG.bit.OVF) {
        TCC1->INTFLAG.bit.OVF = 1;
        TimerB.isrCallback();
    }
}

void TCC2_Handler() {
    if(TCC2->INTFLAG.bit.OVF) {
        TCC2->INTFLAG.bit.OVF = 1;
        TimerC.isrCallback();
    }
}
#elif defined(ATMEGA2560)

#include "ATMEGA2560/Timer.h"

Timer TimerA(1);
Timer TimerB(3);
Timer TimerC(4);
Timer TimerD(5);

ISR(TIMER1_OVF_vect)
{
    TimerA.isrCallback();
}

ISR(TIMER3_OVF_vect)
{
    TimerB.isrCallback();
}

ISR(TIMER4_OVF_vect)
{
    TimerC.isrCallback();
}

ISR(TIMER5_OVF_vect)
{
    TimerD.isrCallback();
}

#elif defined(ATMEGA328)

#include "ATMEGA328/Timer.h"

Timer TimerA(0);
Timer TimerB(1);
Timer TimerC(2);

ISR(TIMER0_OVF_vect)
{
    TimerA.isrCallback();
}

ISR(TIMER1_OVF_vect)
{
    TimerB.isrCallback();
}

ISR(TIMER2_OVF_vect)
{
    TimerC.isrCallback();
}

#endif