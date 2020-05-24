#ifndef VirtualTimer_h
#define VirtualTimer_h

class VirtualTimer
{
public:
    virtual void initialize() = 0;
    virtual void setPeriod(unsigned long microseconds) = 0;
    virtual void start() = 0;
	virtual void stop() = 0;

    virtual void attachInterrupt(void (*isr)()) = 0;
    virtual void detachInterrupt() = 0;

    static void isrDefaultUnused() {};
private:

};



#endif