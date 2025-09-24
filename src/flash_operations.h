#include <msp430.h>
#include <stdint.h>
#include "event_timer.h"

#pragma CODE_SECTION(flash_segment_partial_erase_4, ".flash_operations_section")
#pragma CODE_SECTION(end_flash_segment_partial_erase_4, ".flash_operations_section")
#pragma CODE_SECTION(flash_word_partial_write_10, ".flash_operations_section")
#pragma CODE_SECTION(end_flash_word_partial_write_10, ".flash_operations_section")

//-------------------------------------------------------------------//
// flash_operations.h
//-------------------------------------------------------------------//
// This header file contains functions for manipulating the flash
// memory on F5529 Lauchpad
// NOTES: 
// Intended for use with C11
// functions prepended with end are dummy functions for
//    determining the length of a function
// CODE_SECTION pragma is used to ensure that the functions are placed
//    sequentially
//-------------------------------------------------------------------//

typedef struct flash_stats_struct {
  unsigned int total_bit_errors;
  unsigned int write_latency;
  unsigned int erase_latency;
} flash_stats_s;

typedef struct flash_segment_struct {
  uint16_t* startAddress;
  uint16_t size;// size in bytes!!!!
} flash_segment_s;

void flash_segment_erase(flash_segment_s seg);
void flash_segment_erase_timed(flash_segment_s seg);

void flash_word_write(uint16_t value, uint16_t* targetPtr);
void flash_word_write_timed(uint16_t value, uint16_t* targetPtr);

void flash_segment_partial_erase_4(uint16_t* targetPtr);
void end_flash_segment_partial_erase_4(void);

void flash_word_partial_write_10(uint16_t partialValue, uint16_t* targetPtr);
void end_flash_word_partial_write_10(void);

void flash_stress_segment(flash_segment_s seg, uint16_t val, uint16_t iterations);

void flash_segment_stats(flash_segment_s seg, flash_stats_s* stats, uint16_t test_value);
