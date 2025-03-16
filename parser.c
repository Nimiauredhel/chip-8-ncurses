#include "parser.h"

uint8_t parse(uint8_t first, uint8_t second)
{
    uint8_t nibbA = first >> 4;
    uint8_t nibbB = (first << 4) >> 4;
    uint8_t nibbC = second >> 4;
    uint8_t nibbD = (second << 4) >> 4;

    return 0;
}
