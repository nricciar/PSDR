// PSK31 decoding from https://github.com/zenmetsu/radioModem adapted for the sm32f4 processor
// credit Jason Westervelt (zenmetsu) vespira@gmail.com

//#include <libmaple_types.h>
#include <stdint.h>

#ifndef UTILITY_H_
#define UTILITY_H_

#define Fosc 72000000
//#define Fosc 6950000
//#define Fy (Fosc / 2)
#define SAMPLE_RATE 8192

uint16_t bitrev(uint16_t in, uint8_t bits);

#endif
