#ifndef ArduinoTimers_h
#define ArduinoTimers_h

// This library is designed to use platformio, with build flags ATSAMD21G, ATMEGA2560, ATSAM3X8E, or ATMEGA328 defined in the build options (paltformio.ini)

#if defined(ATSAMD21G)
    #include "ATSAMD21G/Timer.h"
#elif defined(ATMEGA2560)
    #include "ATMEGA2560/Timer.h"
#elif defined(ATSAM3X8E)
    #error "ATSAM3X8E (Due) support for ArduinoTimers has not been written yet"
#elif defined(ATMEGA328)
    #error "ATMEGA328 (Uno) support for ArduinoTimers has not been written yet"
#else
    #error "Cannot compile - please define one of the board constants in the ArduinoTimers library (ATSAMD21G, ATMEGA2560, ATSAM3X8E, ATMEGA328) as a build flag in platformIO"
#endif


#endif