#include <msp430.h>
#include <stdint.h>
#include "event_timer.h"
#include "SRAM_subroutine_copy.h"

#pragma CODE_SECTION(f_segment_partial_erase_4, ".f_segment_partial_erase_4")
#pragma CODE_SECTION(end_f_segment_partial_erase_4, ".f_segment_partial_erase_4")

#pragma CODE_SECTION(f_word_partial_write_0, ".f_word_partial_write_0")
#pragma CODE_SECTION(end_f_word_partial_write_0, ".f_word_partial_write_0")
#pragma CODE_SECTION(f_word_partial_write_4, ".f_word_partial_write_4")
#pragma CODE_SECTION(end_f_word_partial_write_4, ".f_word_partial_write_4")
#pragma CODE_SECTION(f_word_partial_write_6, ".f_word_partial_write_6")
#pragma CODE_SECTION(end_f_word_partial_write_6, ".f_word_partial_write_6")
#pragma CODE_SECTION(f_word_partial_write_8, ".f_word_partial_write_8")
#pragma CODE_SECTION(end_f_word_partial_write_8, ".f_word_partial_write_8")
#pragma CODE_SECTION(f_word_partial_write_10, ".f_word_partial_write_10")
#pragma CODE_SECTION(end_f_word_partial_write_10, ".f_word_partial_write_10")

#pragma CODE_SECTION(f_block_set, ".f_block_set")
#pragma CODE_SECTION(end_f_block_set, ".f_block_set")

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
// .flash_operations_section MUST BE DEFINED IN LINKER COMMAND FILE
//-------------------------------------------------------------------//

typedef struct f_stats_struct {
  unsigned int total_bit_errors;
  unsigned int write_latency;
  unsigned int erase_latency;
} f_stats_s;

typedef struct f_segment_struct {
  uint16_t* startAddress;
  uint16_t size;// size in bytes!!!!
} f_segment_s;

void f_segment_erase(uint16_t* segPtr);
void f_segment_erase_timed(uint16_t* segPtr);


void f_word_write(uint16_t value, uint16_t* targetPtr);
void f_word_write_timed(uint16_t value, uint16_t* targetPtr);


void f_safe_word_write(uint16_t value, uint16_t* targetPtr, f_segment_s seg);


void f_block_set(uint16_t value, uint16_t* blockPtr);
void end_f_block_set(void);


void f_segment_partial_erase_4(uint16_t* targetPtr);
void end_f_segment_partial_erase_4(void);


void   f_word_partial_write_0(uint16_t partialValue, uint16_t* targetPtr);
void   end_f_word_partial_write_0(void);

void   f_word_partial_write_4(uint16_t partialValue, uint16_t* targetPtr);
void   end_f_word_partial_write_4(void);

void   f_word_partial_write_6(uint16_t partialValue, uint16_t* targetPtr);
void   end_f_word_partial_write_6(void);

void   f_word_partial_write_8(uint16_t partialValue, uint16_t* targetPtr);
void   end_f_word_partial_write_8(void);

void   f_word_partial_write_10(uint16_t partialValue, uint16_t* targetPtr);
void   end_f_word_partial_write_10(void);


void f_stress_segment(f_segment_s seg, uint16_t val, uint16_t iterations);


void f_segment_stats(f_segment_s seg, f_stats_s* stats, uint16_t test_value);


void f_segment_stats_partial_erase(f_segment_s seg, f_stats_s* stats, uint16_t test_value);
