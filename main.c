/***************************************************************
* FILENAME: main.c
* DESCRIPTION: This program is intended to demonstrate the
* non-deterministic nature of partial erase operations in flash
* AUTHOR: Jack Pyburn
* DATE: 09/20/2025
* STATUS: Program functional but unexpected output. Flash is 
* being properly erased despite the minimum time violation
****************************************************************/
#include <msp430.h> 
#include "src/flash_operations.h"
#include "src/SRAM_subroutine_copy.h"
#include "src/Serial.h"
#include "src/event_timer.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define INFO_FLASH_SEG_C_START 0x001880

void init_and_wait(void);


int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;	// stop watchdog timer
  init_and_wait(); // holds program until user presses KEY1

  Serial0_setup();

  uint16_t* info_C_start = (void*)INFO_FLASH_SEG_C_START;

  ///////////////////////////////////
  // Erase and reset entire segment
  ///////////////////////////////////

  flash_segment_erase(info_C_start);
  for(int i = 0; i < 64; i++)
    flash_word_write(0x0000, info_C_start + i);

  /////////////////////////////////
  // allocate memory for function
  /////////////////////////////////
  char* space = (char*) malloc((void*)end_flash_segment_partial_erase_4 \
      - (void*)flash_segment_partial_erase_4 + 1);

  if (space == NULL)
    return 1; // not enough room for function in the heap

  //////////////////////////////
  // copy function to SRAM
  //////////////////////////////
  copy_subroutine(flash_segment_partial_erase_4, \
      end_flash_segment_partial_erase_4, space);

  void (*SRAM_flash_seg_part_erase_4)(uint16_t*); // declare function pointer
  SRAM_flash_seg_part_erase_4 = (void*)space;
  
  //////////////////////////////
  // call SRAM copied function
  //////////////////////////////
  event_timer_start();
  SRAM_flash_seg_part_erase_4(info_C_start);
  event_timer_stop();

  free(space); // deallocate memory

  char outStr[30];
  sprintf(outStr, "Time of erase operation: %u uS\n", _event_timer_value);
  Serial0_write(outStr);

  while(1);
	
  return 0;
}

void init_and_wait(void)
{
  P1REN |= BIT1;
  P1DIR |= BIT0;
  P1OUT |= BIT1 + BIT0;
  while(P1IN & BIT1);// wait till button press to start
  P1OUT &= ~BIT0;
}

