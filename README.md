# ArduinoTimers
This library is similar to TimerThree, but written so that you can use Timer 1, 3, 4, or 5 on the mega or TCC0, 1, and 2 on the SAMD21 concurrently.
It is still in the early stages of development, it is relatively inefficient at the moment. 

To use this library, copy the url ```https://github.com/davidcutting42/ArduinoTimers.git#master``` into ```lib_deps``` on platformIO, and include it with ```#include <ArduinoTimers.h>``` in your main.cpp file. 

## ATMEGA2560 Usage

```c
Timer1.initialize();
Timer1.setPeriod(microseconds);
Timer1.attachInterrupt(your_interrupt_handler_function);
Timer1.start();
```
Replace Timer1 with Timer3, Timer4, or Timer5 and the functionality is the same.

## ATSAMD21 Usage

```c
TCC0.initialize();
TCC0.setPeriod(microseconds);
TCC0.attachInterrupt(your_interrupt_handler_function);
TCC0.start();
```
Replace TCC0 with TCC1 or TCC2 and the functionality is the same. TCC3 is not currently supported.
