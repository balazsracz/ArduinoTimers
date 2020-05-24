#ifdef ATSAMD21G

#include "TimerTCC2.h"

TimerTCC2 TimerTCC2Inst;

void (*TimerTCC2::isrCallback)() = TimerTCC2::isrDefaultUnused;

void TCC2_Handler() {
    TimerTCC2Inst.isrCallback();
}

#endif