#include "SRAM_subroutine_copy.h"

void copy_subroutine(char* src_start, char* src_end, char* dst_start)
// src_start is the start address of the subroutine to copy
// src_end is the start address of the following function
// dst_start is the pointer to the destination
// USER MUST ENSURE THAT THE DESTINATION HAS ENOUGH ROOM
{
  while(src_start < src_end){
    *dst_start = *src_start;
    dst_start++;
    src_start++;
  }
}
