#ifndef ATMEGA2560Timer_h
#define ATMEGA2560Timer_h

#include "../VirtualTimer.h"
#include <Arduino.h>

class Timer : public VirtualTimer {
private:
    int pwmPeriod;
    unsigned long timer_resolution;
    unsigned char clockSelectBits;
    int timer_num;
public:
void (*isrCallback)();
    Timer(int timer_num) {
        switch (timer_num)
        {
        case 1:
        case 3:
        case 4:
        case 5:
            timer_resolution = 65536;
            break;
        }
        this->timer_num = timer_num;
    }

    void initialize() {
        switch (timer_num)
        {
        case 1:
            TCCR1B = _BV(WGM13);
            TCCR1A = 0;
            break;
        case 3:
            TCCR3B = _BV(WGM33);
            TCCR3A = 0;
            break;
        case 4:
            TCCR4B = _BV(WGM43);
            TCCR4A = 0;
            break;
        case 5:
            TCCR5B = _BV(WGM53);
            TCCR5A = 0;
            break;
        }
    }
    void setPeriod(unsigned long microseconds) {
        const unsigned long cycles = (F_CPU / 2000000) * microseconds;
        if (cycles < timer_resolution) {
            switch (timer_num)
            {
            case 1:
                clockSelectBits = _BV(CS10);
                break;
            case 3:
                clockSelectBits = _BV(CS30);
                break;
            case 4:
                clockSelectBits = _BV(CS40);
                break;
            case 5:
                clockSelectBits = _BV(CS50);
                break;
            }
            pwmPeriod = cycles;
        } else
        if (cycles < timer_resolution * 8) {
            switch (timer_num)
            {
            case 1:
                clockSelectBits = _BV(CS11);
                break;
            case 3:
                clockSelectBits = _BV(CS31);
                break;
            case 4:
                clockSelectBits = _BV(CS41);
                break;
            case 5:
                clockSelectBits = _BV(CS51);
                break;
            }
            pwmPeriod = cycles / 8;
        } else
        if (cycles < timer_resolution * 64) {
            switch (timer_num)
            {
            case 1:
                clockSelectBits = _BV(CS11) | _BV(CS10);
                break;
            case 3:
                clockSelectBits = _BV(CS31) | _BV(CS30);
                break;
            case 4:
                clockSelectBits = _BV(CS41) | _BV(CS40);
                break;
            case 5:
                clockSelectBits = _BV(CS51) | _BV(CS50);
                break;
            }
            pwmPeriod = cycles / 64;
        } else
        if (cycles < timer_resolution * 256) {
            switch (timer_num)
            {
            case 1:
                clockSelectBits = _BV(CS12);
                break;
            case 3:
                clockSelectBits = _BV(CS32);
                break;
            case 4:
                clockSelectBits = _BV(CS42);
                break;
            case 5:
                clockSelectBits = _BV(CS52);
                break;
            }
            pwmPeriod = cycles / 256;
        } else
        if (cycles < timer_resolution * 1024) {
            switch (timer_num)
            {
            case 1:
                clockSelectBits = _BV(CS12) | _BV(CS10);
                break;
            case 3:
                clockSelectBits = _BV(CS32) | _BV(CS30);
                break;
            case 4:
                clockSelectBits = _BV(CS42) | _BV(CS40);
                break;
            case 5:
                clockSelectBits = _BV(CS52) | _BV(CS50);
                break;
            }
            pwmPeriod = cycles / 1024;
        } else {
            switch (timer_num)
            {
            case 1:
                clockSelectBits = _BV(CS12) | _BV(CS10);
                break;
            case 3:
                clockSelectBits = _BV(CS32) | _BV(CS30);
                break;
            case 4:
                clockSelectBits = _BV(CS42) | _BV(CS40);
                break;
            case 5:
                clockSelectBits = _BV(CS52) | _BV(CS50);
                break;
            }
            pwmPeriod = timer_resolution - 1;
        }

        switch (timer_num)
        {
        case 1:
            ICR1 = pwmPeriod;
            TCCR1B = _BV(WGM13) | clockSelectBits;
            break;
        case 3:
            ICR3 = pwmPeriod;
            TCCR3B = _BV(WGM33) | clockSelectBits;
            break;
        case 4:
            ICR4 = pwmPeriod;
            TCCR4B = _BV(WGM43) | clockSelectBits;
            break;
        case 5:
            ICR5 = pwmPeriod;
            TCCR5B = _BV(WGM53) | clockSelectBits;
            break;
        }
        
    }
    void start() {
        switch (timer_num)
        {
        case 1:
            TCCR1B = 0;
            TCNT1 = 0;		// TODO: does this cause an undesired interrupt?
            TCCR1B = _BV(WGM13) | clockSelectBits;
            break;
        case 3:
            TCCR3B = 0;
            TCNT3 = 0;		// TODO: does this cause an undesired interrupt?
            TCCR3B = _BV(WGM33) | clockSelectBits;
            break;
        case 4:
            TCCR4B = 0;
            TCNT4 = 0;		// TODO: does this cause an undesired interrupt?
            TCCR4B = _BV(WGM43) | clockSelectBits;
            break;
        case 5:
            TCCR5B = 0;
            TCNT5 = 0;		// TODO: does this cause an undesired interrupt?
            TCCR5B = _BV(WGM53) | clockSelectBits;
            break;
        }
        
        
    }
    void stop() {
        switch (timer_num)
        {
        case 1:
            TCCR1B = _BV(WGM13);
            break;
        case 3:
            TCCR3B = _BV(WGM33);
            break;
        case 4:
            TCCR4B = _BV(WGM43);
            break;
        case 5:
            TCCR5B = _BV(WGM53);
            break;
        }        
    }

    void attachInterrupt(void (*isr)()) {
        isrCallback = isr;
	    
        switch (timer_num)
        {
        case 1:
            TIMSK1 = _BV(TOIE1); 
            break;
        case 3:
            TIMSK3 = _BV(TOIE3); 
            break;
        case 4:
            TIMSK4 = _BV(TOIE4); 
            break;
        case 5:
            TIMSK5 = _BV(TOIE5); 
            break;
        }   
    }
    
    void detachInterrupt() {
        switch (timer_num)
        {
        case 1:
            TIMSK1 = 0; 
            break;
        case 3:
            TIMSK3 = 0; 
            break;
        case 4:
            TIMSK4 = 0; 
            break;
        case 5:
            TIMSK5 = 0; 
            break;
        }  
    }

};

extern Timer Timer1;
extern Timer Timer3;
extern Timer Timer4;
extern Timer Timer5;



#endif