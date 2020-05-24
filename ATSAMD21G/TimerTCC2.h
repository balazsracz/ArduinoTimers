#ifndef ATSAMD21GTimer_h
#define ATSAMD21GTimer_h

#include "../VirtualTimer.h"
#include <Arduino.h>

#define TCC2_RESOLUTION 65536UL

class TimerTCC2 : public VirtualTimer
{
public:
    void initialize() {
        REG_GCLK_GENDIV =   GCLK_GENDIV_DIV(1) |            // Divide 48MHz by 1
                            GCLK_GENDIV_ID(4);              // Apply to GCLK4
        while (GCLK->STATUS.bit.SYNCBUSY);                  // Wait for synchronization
            
        REG_GCLK_GENCTRL =  GCLK_GENCTRL_GENEN |            // Enable GCLK
                            GCLK_GENCTRL_SRC_DFLL48M |      // Set the 48MHz clock source
                            GCLK_GENCTRL_ID(4);             // Select GCLK4
        while (GCLK->STATUS.bit.SYNCBUSY);                  // Wait for synchronization
        
        REG_GCLK_CLKCTRL =  GCLK_CLKCTRL_CLKEN |            // Enable generic clock
                            4 << GCLK_CLKCTRL_GEN_Pos |     // Apply to GCLK4
                            GCLK_CLKCTRL_ID_TCC2_TC3;      // Feed GCLK to TCC2/1
        while (GCLK->STATUS.bit.SYNCBUSY);                  // Wait for synchronization
        
        TCC2->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM;             // Select NPWM as waveform
        while (TCC2->SYNCBUSY.bit.WAVE);                    // Wait for synchronization

    }
    void setPeriod(unsigned long microseconds) {
        const unsigned long cycles = F_CPU / 1000000 * microseconds;    // cycles corresponds to how many clock ticks per microsecond times number of microseconds we want
        if(cycles < TCC2_RESOLUTION) {
            TCC2->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV1_Val);
            pwmPeriod = cycles;
        } else
        if(cycles < TCC2_RESOLUTION * 2) {
            TCC2->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV2_Val);
            pwmPeriod = cycles / 2;
        } else
        if(cycles < TCC2_RESOLUTION * 4) {
            TCC2->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV4_Val);
            pwmPeriod = cycles / 4;
        } else
        if(cycles < TCC2_RESOLUTION * 8) {
            TCC2->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV8_Val);
            pwmPeriod = cycles / 8;
        } else
        if(cycles < TCC2_RESOLUTION * 16) {
            TCC2->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV16_Val);
            pwmPeriod = cycles / 16;
        } else
        if(cycles < TCC2_RESOLUTION * 64) {
            TCC2->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV64_Val);
            pwmPeriod = cycles / 64;
        } else
        if(cycles < TCC2_RESOLUTION * 1024) {
            TCC2->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV1024_Val);
            pwmPeriod = cycles / 1024;
        }
        TCC2->PER.reg = pwmPeriod;
        while (TCC2->SYNCBUSY.bit.PER);
    }
    void start() {
        TCC2->CTRLA.bit.ENABLE = 1;                         // Turn on the output
        while (TCC2->SYNCBUSY.bit.ENABLE);                  // Wait for synchronization
    }
    void stop() {
        TCC2->CTRLA.bit.ENABLE = 0;                         // Turn on the output
        while (TCC2->SYNCBUSY.bit.ENABLE);                  // Wait for synchronization
    }

    void attachInterrupt(void (*isr)()) {
        isrCallback = isr;                                  // Store the interrupt callback function
        TCC2->INTENSET.reg = TCC_INTENSET_OVF;              // Set the interrupt to occur on overflow
        NVIC_EnableIRQ((IRQn_Type) TCC2_IRQn);              // Enable the interrupt (clock is still off)    
    }
    
    void detachInterrupt() {
        NVIC_DisableIRQ((IRQn_Type) TCC2_IRQn);              // Enable the interrupt (clock is still off)    
    }

    static void (*isrCallback)();
private:
    static unsigned short pwmPeriod;
};

#endif