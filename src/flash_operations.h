#include <msp430.h>
#include <stdint.h>

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

void flash_segment_erase(uint16_t* targetPtr);

void flash_word_write(uint16_t value, uint16_t* targetPtr);

void flash_segment_partial_erase_4(uint16_t* targetPtr);
void end_flash_segment_partial_erase_4(void);

void flash_word_partial_write_10(uint16_t partialValue, uint16_t* targetPtr);
void end_flash_word_partial_write_10(void);
