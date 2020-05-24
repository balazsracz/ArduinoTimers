#ifdef ATSAMD21G

#include "TimerTCC1.h"

TimerTCC1 TimerTCC1Inst;

void (*TimerTCC1::isrCallback)() = TimerTCC1::isrDefaultUnused;

void TCC1_Handler() {
    TimerTCC1Inst.isrCallback();
}

#endif