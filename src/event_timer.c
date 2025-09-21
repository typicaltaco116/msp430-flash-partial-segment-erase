#include "event_timer.h"


void event_timer_start(void)
{
  TA0CTL |= TACLR;
  TA0CTL |= TASSEL_1 + ID__1 + MC_2;
  // SMCLK = 1.048MHz, no divider, continous mode
}

void event_timer_stop(void)
{
  TA0CTL &= ~MC_0; // halt timer
  _event_timer_value = TA0R;
}

