#if defined(ATSAMD21G)

#include "ATSAMD21G/Timer.h"

Timer TimerTCC0(TCC0);
Timer TimerTCC1(TCC1);
Timer TimerTCC2(TCC2);

void TCC0_Handler() {
    if(TCC0->INTFLAG.bit.OVF) {
        TCC0->INTFLAG.bit.OVF = 1;
        TimerTCC0.isrCallback();
    }
}

void TCC1_Handler() {
    if(TCC1->INTFLAG.bit.OVF) {
        TCC1->INTFLAG.bit.OVF = 1;
        TimerTCC1.isrCallback();
    }
}

void TCC2_Handler() {
    if(TCC2->INTFLAG.bit.OVF) {
        TCC2->INTFLAG.bit.OVF = 1;
        TimerTCC2.isrCallback();
    }
}
#elif defined(ATMEGA2560)

#include "ATMEGA2560/Timer.h"

Timer Timer1(1);
Timer Timer3(3);
Timer Timer4(4);
Timer Timer5(5);

ISR(TIMER1_OVF_vect)
{
    interrupts();
    Timer1.isrCallback();
}

ISR(TIMER3_OVF_vect)
{
    interrupts();
    Timer3.isrCallback();
}

ISR(TIMER4_OVF_vect)
{
    interrupts();
    Timer4.isrCallback();
}

ISR(TIMER5_OVF_vect)
{
    interrupts();
    Timer5.isrCallback();
}



#endif