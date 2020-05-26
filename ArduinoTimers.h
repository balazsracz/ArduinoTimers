#ifndef ArduinoTimers_h
#define ArduinoTimers_h

#if defined(ARDUINO_SAMD_ZERO)
    #include "ATSAMD21G/Timer.h"
#elif defined(ARDUINO_AVR_MEGA) || defined(ARDUINO_AVR_MEGA2560)
    #include "ATMEGA2560/Timer.h"
#elif defined(ARDUINO_AVR_UNO)
    #include "ATMEGA328/Timer.h"
#else
    #error "Cannot compile - ArduinoTimers library does not support your board, or you are missing compatible build flags."
#endif

#endif