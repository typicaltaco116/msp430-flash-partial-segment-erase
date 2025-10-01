#include <msp430.h>
/*****************************************************************
* FILENAME: event_timer.h
* DESCRIPTION: Intended to record the length of very fast events
* Timer will overflow after only 65.536 mS by default
* RESOURCE USAGE: Timer A0, no interrupts atm
******************************************************************/

#define EVENT_TIMER_START {\
  TA0CTL |= TACLR;\
  TA0CTL |= TASSEL_2 + ID__1 + MC_2;\
}
#define EVENT_TIMER_STOP {\
  _event_timer_value = TA0R - 4;\
  TA0CTL &= ~MC_3;\
}

extern unsigned int _event_timer_value;

void event_timer_start(void);

void event_timer_stop(void);
