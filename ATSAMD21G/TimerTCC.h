#ifndef ATSAMD21GTimer_h
#define ATSAMD21GTimer_h

#include "../VirtualTimer.h"
#include <Arduino.h>

class TimerTCC : public VirtualTimer
{
private:
    int pwmPeriod;
    unsigned long timer_resolution;
public:
    void (*isrCallback)();
    Tcc* timer;

    TimerTCC(Tcc* timer) {
        this->timer = timer;
        if(timer == TCC0 || timer == TCC1) {
            timer_resolution = 16777216;
        } else {
            timer_resolution = 65536;
        }
    }

    void initialize() {
        if(timer == TCC0 || timer == TCC1) {
            REG_GCLK_GENDIV =   GCLK_GENDIV_DIV(1) |            // Divide 48MHz by 1
                                GCLK_GENDIV_ID(4);              // Apply to GCLK4
            while (GCLK->STATUS.bit.SYNCBUSY);                  // Wait for synchronization
                
            REG_GCLK_GENCTRL =  GCLK_GENCTRL_GENEN |            // Enable GCLK
                                GCLK_GENCTRL_SRC_DFLL48M |      // Set the 48MHz clock source
                                GCLK_GENCTRL_ID(4);             // Select GCLK4
            while (GCLK->STATUS.bit.SYNCBUSY);                  // Wait for synchronization
            
            REG_GCLK_CLKCTRL =  GCLK_CLKCTRL_CLKEN |            // Enable generic clock
                                4 << GCLK_CLKCTRL_GEN_Pos |     // Apply to GCLK4
                                GCLK_CLKCTRL_ID_TCC0_TCC1;      // Feed GCLK to TCC0/1
            while (GCLK->STATUS.bit.SYNCBUSY);                  // Wait for synchronization
        } 
        else if (timer == TCC2) {
            REG_GCLK_GENDIV =   GCLK_GENDIV_DIV(1) |            // Divide 48MHz by 1
                                GCLK_GENDIV_ID(5);              // Apply to GCLK4
            while (GCLK->STATUS.bit.SYNCBUSY);                  // Wait for synchronization
                
            REG_GCLK_GENCTRL =  GCLK_GENCTRL_GENEN |            // Enable GCLK
                                GCLK_GENCTRL_SRC_DFLL48M |      // Set the 48MHz clock source
                                GCLK_GENCTRL_ID(5);             // Select GCLK4
            while (GCLK->STATUS.bit.SYNCBUSY);                  // Wait for synchronization
            
            REG_GCLK_CLKCTRL =  GCLK_CLKCTRL_CLKEN |            // Enable generic clock
                                5 << GCLK_CLKCTRL_GEN_Pos |     // Apply to GCLK4
                                GCLK_CLKCTRL_ID_TCC2_TC3;      // Feed GCLK to TCC0/1
            while (GCLK->STATUS.bit.SYNCBUSY);                  // Wait for synchronization
        }
        
        
        timer->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM;             // Select NPWM as waveform
        while (timer->SYNCBUSY.bit.WAVE);                    // Wait for synchronization

    }
    void setPeriod(unsigned long microseconds) {
        const unsigned long cycles = F_CPU / 1000000 * microseconds;    // cycles corresponds to how many clock ticks per microsecond times number of microseconds we want
        if(cycles < timer_resolution) {
            TCC0->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV1_Val);
            pwmPeriod = cycles;
        } else
        if(cycles < timer_resolution * 2) {
            TCC0->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV2_Val);
            pwmPeriod = cycles / 2;
        } else
        if(cycles < timer_resolution * 4) {
            TCC0->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV4_Val);
            pwmPeriod = cycles / 4;
        } else
        if(cycles < timer_resolution * 8) {
            TCC0->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV8_Val);
            pwmPeriod = cycles / 8;
        } else
        if(cycles < timer_resolution * 16) {
            TCC0->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV16_Val);
            pwmPeriod = cycles / 16;
        } else
        if(cycles < timer_resolution * 64) {
            TCC0->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV64_Val);
            pwmPeriod = cycles / 64;
        } else
        if(cycles < timer_resolution * 1024) {
            TCC0->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV1024_Val);
            pwmPeriod = cycles / 1024;
        }
        TCC0->PER.reg = pwmPeriod;
        while (TCC0->SYNCBUSY.bit.PER);
    }
    void start() {
        TCC0->CTRLA.bit.ENABLE = 1;                         // Turn on the output
        while (TCC0->SYNCBUSY.bit.ENABLE);                  // Wait for synchronization
    }
    void stop() {
        TCC0->CTRLA.bit.ENABLE = 0;                         // Turn on the output
        while (TCC0->SYNCBUSY.bit.ENABLE);                  // Wait for synchronization
    }

    void attachInterrupt(void (*isr)()) {
        isrCallback = isr;                                  // Store the interrupt callback function
        TCC0->INTENSET.reg = TCC_INTENSET_OVF;              // Set the interrupt to occur on overflow
        NVIC_EnableIRQ((IRQn_Type) TCC0_IRQn);              // Enable the interrupt (clock is still off)    
    }
    
    void detachInterrupt() {
        NVIC_DisableIRQ((IRQn_Type) TCC0_IRQn);              // Enable the interrupt (clock is still off)    
    }
};

TimerTCC TimerTCC0(TCC0);
TimerTCC TimerTCC1(TCC1);
TimerTCC TimerTCC2(TCC2);

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

#endif