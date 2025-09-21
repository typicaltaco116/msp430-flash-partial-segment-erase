#include <msp430.h>
/*****************************************************************
* FILENAME: event_timer.h
* DESCRIPTION: Intended to record the length of very fast events
* Timer will overflow after only 65.536 mS by default
* RESOURCE USAGE: Timer A1 is in use, no interrupts atm
******************************************************************/

unsigned int _event_timer_value = 0;

void event_timer_start(void);

void event_timer_stop(void);

//unsigned int event_timer_value(void);
