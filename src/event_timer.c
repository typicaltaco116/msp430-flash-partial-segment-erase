#include "event_timer.h"

unsigned int _event_timer_value = 0;

void event_timer_start(void)
{
  TA0CTL |= TACLR;
  TA0CTL |= TASSEL_2 + ID__1 + MC_2;
  // SMCLK = 1.048MHz, no divider, continous mode
}

void event_timer_stop(void)
{
  TA0CTL &= ~MC_0; // halt timer
  _event_timer_value = TA0R;
}

