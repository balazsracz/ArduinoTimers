#ifndef ArduinoTimers_h
#define ArduinoTimers_h

// This library is designed to use platformio, with build flags ATSAMD21G, ATMEGA2560, ATSAM3X8E, or ATMEGA328 defined in the build options (paltformio.ini)

#if defined(ATSAMD21G)
    #include "ATSAMD21G/TimerTCC0.h"
    #include "ATSAMD21G/TimerTCC1.h"
    #include "ATSAMD21G/TimerTCC2.h"
    // TODO update CMSIS to enable support for TCC3. Big project.
#elif defined(ATMEGA2560)
    
#elif defined(ATSAM3X8E)

#elif defined(ATMEGA328)

#endif



#endif