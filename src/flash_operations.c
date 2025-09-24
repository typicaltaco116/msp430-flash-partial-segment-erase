#include "flash_operations.h"

void flash_segment_erase(flash_segment_s seg)
{
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + ERASE; // enable segment erase
  *seg.startAddress = 0x0000; // dummy write to initiate erase

  while(FCTL3 & BUSY); // loop while busy
  // not really necessary when executing from flash

  FCTL1 = FWPW; // clear ERASE
  FCTL3 = FWPW + LOCK; // lock
}

void flash_segment_erase_timed(flash_segment_s seg)
{
  while(FCTL3 & BUSY);

  event_timer_start();
  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + ERASE; // enable segment erase
  *seg.startAddress = 0x0000; // dummy write to initiate erase

  while(FCTL3 & BUSY); // loop while busy
  // not really necessary when executing from flash
  event_timer_stop();

  FCTL1 = FWPW; // clear ERASE
  FCTL3 = FWPW + LOCK; // lock
}

void flash_word_write(uint16_t value, uint16_t* targetPtr)
{
  while(FCTL3 & BUSY);

  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = value; // write value

  while(FCTL3 & BUSY);

  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
}

void flash_word_write_timed(uint16_t value, uint16_t* targetPtr)
{
  while(FCTL3 & BUSY);

  event_timer_start();
  FCTL3 = FWPW; //clear lock
  FCTL1 = FWPW + WRT; // enable word write
  *targetPtr = value; // write value

  while(FCTL3 & BUSY);
  event_timer_stop();

  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
}

void flash_segment_partial_erase_4(uint16_t* targetPtr)
// THIS FUNCTION MUST BE EXECUTED FROM RAM
// segment erase takes a very long time 23 - 32 ms for the F5529
// emergency exit after 10 us
{
  while(FCTL3 & BUSY);

  FCTL3 = FWPW;          // clear lock
  FCTL1 = FWPW + ERASE; // enable segment erase
  *targetPtr = 0x0000; // dummy write to initiate erase

  __no_operation(); // 4x NOP instructions
  __no_operation(); // ~4 us delay
  __no_operation();
  __no_operation();

  FCTL3 = FWPW + EMEX; // emergency exit
  FCTL1 = FWPW; // clear ERASE
  FCTL3 = FWPW + LOCK; // lock
}
void end_flash_segment_partial_erase_4(void) {}

void flash_word_partial_write_10(uint16_t partialValue, uint16_t* targetPtr)
// THIS FUNCTION MUST BE EXECUTED FROM RAM
// word write takes about 64 - 85 us for the F5529
// emergency exit after 10 us
{
  while(FCTL3 & BUSY);

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
  FCTL1 = FWPW; // clear WRT
  FCTL3 = FWPW + LOCK; // lock
}
void end_flash_word_partial_write_10(void) {}


void flash_stress_segment(flash_segment_s seg, uint16_t val, uint16_t iterations)
// Since erasing flash forces all bits high, a value of 0x0000 will result in
//    the highest possible stresssing of all bits.
{
  uint16_t* target;
  seg.size = seg.size >> 1; // convert to words
  for (int i = 0; i < iterations; i++){
    flash_segment_erase(seg);
    target = seg.startAddress;
    while(target < seg.size + seg.startAddress)
      flash_word_write(val, target++);
  }
}

void flash_segment_stats(flash_segment_s seg, flash_stats_s* stats, uint16_t test_value)
/* flash_segment_s seg: struct for target flash segment
   flash_stats_s* stats: pointer to stats struct for returning the results
   uint16_t test_value: value to compare the read flash to */
{
  // Erase -> write test_value to each byte -> tally bit error rate 
  uint16_t* target = seg.startAddress;
  uint16_t compare_vals;
  stats->total_bit_errors = 0;
  seg.size = seg.size >> 1; // convert to words

  flash_segment_erase_timed(seg);
  stats->erase_latency = _event_timer_value;
 
  while(target < seg.startAddress + seg.size){
    flash_word_write_timed(test_value, target);
    stats->write_latency = _event_timer_value;

    compare_vals = test_value ^ *target; // xor with value just written
    if(!compare_vals)
      for(int i = 0; i < sizeof(compare_vals); i++){
        stats->total_bit_errors += compare_vals & BIT0;
        compare_vals = compare_vals >> 1;
      }

    target++;
  }
}
