// PSK31 decoding from https://github.com/zenmetsu/radioModem adapted for the sm32f4 processor
// credit Jason Westervelt (zenmetsu) vespira@gmail.com

#ifndef BAUDTIMER_H_
#define BAUDTIMER_H_


#include "utility.h"

//Baud timer control functions
void baud_timer_init();
void baud_timer_restart();
unsigned int baud_time_get();

#endif
