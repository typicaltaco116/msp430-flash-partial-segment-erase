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
#include <stdint.h>
#include <stdlib.h>

#define INFO_FLASH_SEG_C_START 0x001880


int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;	// stop watchdog timer

  uint16_t* info_C_start = (void*)INFO_FLASH_SEG_C_START;

  flash_segment_erase(info_C_start);
  for(int i = 0; i < 64; i++)
    flash_word_write(0x0000, info_C_start + i);

  char* space = (char*) malloc((void*)end_flash_segment_partial_erase_4 \
      - (void*)flash_segment_partial_erase_4 + 1);

  if (space == NULL)
    return 1; // not enough room for function in the heap


  copy_subroutine(flash_segment_partial_erase_4, \
      end_flash_segment_partial_erase_4, space);

  void (*SRAM_flash_seg_part_erase_4)(uint16_t*); // declare function pointer
  SRAM_flash_seg_part_erase_4 = (void*)space;

  SRAM_flash_seg_part_erase_4(info_C_start);

  free(space); // deallocate memory

  while(1);
	
  return 0;
}
