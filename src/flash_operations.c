#include "flash_operations.h"

void f_segment_erase(uint16_t* segPtr)
{
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + ERASE; // enable segment erase
  *segPtr = 0x0000; // dummy write to initiate erase

  while(FCTL3 & BUSY); // loop while busy
  // not really necessary when executing from flash

  FCTL1 = FWPW; // clear ERASE
  FCTL3 = FWPW + LOCK; // lock
}


void f_segment_erase_timed(uint16_t* segPtr)
{
  while(FCTL3 & BUSY);

  EVENT_TIMER_START; // defined in event_timer.h
  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + ERASE; // enable segment erase
  *segPtr = 0x0000; // dummy write to initiate erase

  while(FCTL3 & BUSY); // loop while busy
  // not really necessary when executing from flash
  EVENT_TIMER_STOP;

  FCTL1 = FWPW; // clear ERASE
  FCTL3 = FWPW + LOCK; // lock
}


void f_word_write(uint16_t value, uint16_t* targetPtr)
{
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = value; // write value

  while(FCTL3 & BUSY);

  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
}


void f_word_write_timed(uint16_t value, uint16_t* targetPtr)
{
  while(FCTL3 & BUSY);

  EVENT_TIMER_START;

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = value; // write value

  while(FCTL3 & BUSY);

  EVENT_TIMER_STOP;

  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
}


void f_safe_word_write(uint16_t value, uint16_t* targetPtr, f_segment_s seg)
/*
   Function to write a word to a spot in flash memory without invalidating
   areas around it.
   Copies every value in the sector into the heap in order to restore after.
*/
{
  uint16_t* savedArray = (uint16_t*)malloc(seg.size); // allocates LOTS of memory
  // most of the time allocates 512 bytes!
  seg.size = seg.size >> 1; //convert to words

  for(int i = 0; i < seg.size; i++) // copy items over to RAM
    savedArray[i] = seg.startAddress[i];

  savedArray[(targetPtr - seg.startAddress) >> 1] = value;
  // place new value

  f_erase_segment(seg.startAddress);

  for(int i = 0; i < seg.size; i++) // write values
    f_word_write(savedArray[i], seg.startAddress + (i << 1));

  free(savedArray); // deallocate memory
}


void f_block_set(uint16_t value, uint16_t* blockPtr)
// magic numbers for a block size of 512
// Only a 128 byte row can be written at once
// must be executed from RAM
{
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; // clear lock

  for(uint8_t s = 4; s != 0; s--){
    FCTL1 = FWPW + WRT + BLKWRT;

    for(int i = 32; i != 0; i--){
      *(blockPtr++) = value;
      *(blockPtr++) = value;
      while(!(FCTL3 & WAIT));
    }

    FCTL1 = FWPW + WRT; // clear BLKWRT
    while(FCTL3 & BUSY);
  }

  FCTL1 = FWPW; // clear BLKWRT and WRT
  FCTL3 = FWPW + LOCK; // lock
}
void end_f_block_set(void) {}


void f_segment_partial_erase_4(uint16_t* targetPtr)
// THIS FUNCTION MUST BE EXECUTED FROM RAM
// segment erase takes a very long time 23 - 32 ms for the F5529
// emergency exit after 10 us
{
  while(FCTL3 & BUSY);

  EVENT_TIMER_START;
  FCTL3 = FWPW;          // clear lock
  FCTL1 = FWPW + ERASE; // enable segment erase
  *targetPtr = 0x0000; // dummy write to initiate erase

  __no_operation(); // 4x NOP instructions
  __no_operation(); // ~4 us delay
  __no_operation();
  __no_operation();

  FCTL3 = FWPW + EMEX; // emergency exit
  EVENT_TIMER_STOP;
  FCTL1 = FWPW; // clear ERASE
  FCTL3 = FWPW + LOCK; // lock
}
void end_f_segment_partial_erase_4(void) {}


void f_word_partial_write_0(uint16_t partialValue, uint16_t* targetPtr)
// THIS FUNCTION MUST BE EXECUTED FROM RAM
// This function is timed!!! the value in _event_timer_value is the write time
{
  while(FCTL3 & BUSY);

  EVENT_TIMER_START;
  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = partialValue; // write value

  FCTL3 = FWPW + EMEX; // emergency exit
  EVENT_TIMER_STOP;
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
}
void end_f_word_partial_write_0(void) {}

void f_word_partial_write_4(uint16_t partialValue, uint16_t* targetPtr)
{
  while(FCTL3 & BUSY);

  EVENT_TIMER_START;
  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = partialValue; // write value

  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();

  FCTL3 = FWPW + EMEX; // emergency exit
  EVENT_TIMER_STOP;
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
}
void end_f_word_partial_write_4(void) {}

void f_word_partial_write_6(uint16_t partialValue, uint16_t* targetPtr)
{
  while(FCTL3 & BUSY);

  EVENT_TIMER_START;
  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = partialValue; // write value

  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();

  FCTL3 = FWPW + EMEX; // emergency exit
  EVENT_TIMER_STOP;
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
}
void end_f_word_partial_write_6(void) {}

void f_word_partial_write_8(uint16_t partialValue, uint16_t* targetPtr)
// THIS FUNCTION MUST BE EXECUTED FROM RAM
// This function is timed!!! the value in _event_timer_value is the write time
{
  while(FCTL3 & BUSY);

  EVENT_TIMER_START;
  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = partialValue; // write value

  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();

  FCTL3 = FWPW + EMEX; // emergency exit
  EVENT_TIMER_STOP;
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
}
void end_f_word_partial_write_8(void) {}

void f_word_partial_write_10(uint16_t partialValue, uint16_t* targetPtr)
// emergency exit after ~10 us
{
  while(FCTL3 & BUSY);

  EVENT_TIMER_START;
  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = partialValue; // write value

  __no_operation(); // 10x NOP instructions
  __no_operation(); // ~10 us delay
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();


  FCTL3 = FWPW + EMEX; // emergency exit
  EVENT_TIMER_STOP;
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
}
void end_f_word_partial_write_10(void) {}


void f_stress_segment(f_segment_s seg, uint16_t val, uint16_t iterations)
// Since erasing flash forces all bits high, a value of 0x0000 will result in
//    the highest possible stresssing of all bits.
{
  uint16_t* target = seg.startAddress;
  void (*SRAM_f_block_set)(uint16_t, uint16_t*); // declare function pointer

  SRAM_f_block_set = malloc_subroutine(f_block_set, end_f_block_set);
  if(!(void*)SRAM_f_block_set)
    return; // null pointer means the memory cannot be allocated

  for (int i = 0; i < iterations; i++){
    f_segment_erase(seg.startAddress);

    SRAM_f_block_set(val, target);
  }

  free((void*)SRAM_f_block_set); // deallocate memory
}


void f_segment_stats(f_segment_s seg, f_stats_s* stats, uint16_t test_value)
/* flash_segment_s seg: struct for target flash segment
   flash_stats_s* stats: pointer to stats struct for returning the results
   uint16_t test_value: value to compare the read flash to */
{
  // Erase -> write test_value to each byte -> tally bit error rate 
  uint16_t* target = seg.startAddress;
  uint16_t compare_vals;
  stats->total_bit_errors = 0;
  seg.size = seg.size >> 1; // convert to words

  f_segment_erase_timed(seg.startAddress);
  stats->erase_latency = _event_timer_value;

  while(target < seg.startAddress + seg.size){
    f_word_write_timed(test_value, target);
    stats->write_latency = _event_timer_value;

    compare_vals = test_value ^ *target; // xor with value just written
    if(compare_vals)
      for(int i = 0; i < 16; i++){
        stats->total_bit_errors += compare_vals & BIT0;
        compare_vals = compare_vals >> 1;
      }

    target++;
  }
}


void f_segment_stats_partial_erase(f_segment_s seg, f_stats_s* stats, uint16_t test_value)
/*
   This function will get the total number of errors after a partial erase operation.
 */
{
  // Erase -> write test_value to each byte -> tally bit error rate 
  uint16_t* target = seg.startAddress;
  uint16_t compare_vals;
  stats->total_bit_errors = 0;
  seg.size = seg.size >> 1; // convert to words

  void (*SRAM_partial_erase_4)(uint16_t*);
  SRAM_partial_erase_4 = malloc_subroutine(f_segment_partial_erase_4,
      end_f_segment_partial_erase_4);

  event_timer_start();
  SRAM_partial_erase_4(seg.startAddress);
  event_timer_stop();

  stats->erase_latency = _event_timer_value - 10;// magic number for overhead time cost of call

  free(SRAM_partial_erase_4); // deallocate memory

  while(target < seg.startAddress + seg.size){
    compare_vals = test_value ^ *target; // xor with value in memory
    if(compare_vals)
      for(int i = 0; i < 16; i++){
        stats->total_bit_errors += compare_vals & BIT0;
        compare_vals = compare_vals >> 1;
      }

    target++;
  }
}
