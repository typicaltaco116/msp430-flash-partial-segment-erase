#include <msp430.h>
/*****************************************************************
* FILENAME: event_timer.h
* DESCRIPTION: Intended to record the length of very fast events
* Timer will overflow after only 65.536 mS by default
* RESOURCE USAGE: Timer A0, no interrupts atm
******************************************************************/

extern unsigned int _event_timer_value;

void event_timer_start(void);

void event_timer_stop(void);

//unsigned int event_timer_value(void);
