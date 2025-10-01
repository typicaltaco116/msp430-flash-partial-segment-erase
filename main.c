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

#define INFO_FLASH_SEG_C_START 0x01880
#define F5529_FLASH_BANK_D    0x1C400     /* FLASH BANK D starts at 0x1_C400 and ends at 0x2_43FF */

void init_and_wait(void);


int main(void)
{
  void (*SRAM_f_seg_part_erase_4)(uint16_t*); // declare function pointer
  char outStr[64];


  WDTCTL = WDTPW + WDTHOLD;	// stop watchdog timer
  init_and_wait(); // holds program until user presses KEY1

  Serial0_setup();

  f_segment_s info_C_seg;
  info_C_seg.startAddress = (void*)INFO_FLASH_SEG_C_START;
  info_C_seg.size = 128;

  f_segment_s bank_D_seg_0;
  bank_D_seg_0.startAddress = (void*)F5529_FLASH_BANK_D;
  bank_D_seg_0.size = 512;

  ///////////////////////////////////
  // Erase and reset entire segment
  ///////////////////////////////////

  f_segment_erase(bank_D_seg_0.startAddress);
  for(int i = 0; i < 256; i++)
    f_word_write(0x0000, bank_D_seg_0.startAddress+ i);


  SRAM_f_seg_part_erase_4 = malloc_subroutine(\
      f_segment_partial_erase_4, end_f_segment_partial_erase_4);
  if (!SRAM_f_seg_part_erase_4)
    return 1; // not enough space on the heap
  
  //////////////////////////////
  // call SRAM copied function
  //////////////////////////////
  SRAM_f_seg_part_erase_4(bank_D_seg_0.startAddress);

  free(SRAM_f_seg_part_erase_4); // deallocate memory

  sprintf(outStr, "Time of erase operation: %u uS\n", _event_timer_value);
  Serial0_write(outStr);

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

