// PSK31 decoding from https://github.com/zenmetsu/radioModem adapted for the sm32f4 processor
// credit Jason Westervelt (zenmetsu) vespira@gmail.com

#ifndef ARCTANAPPROX_H_
#define ARCTANAPPROX_H_

#include "utility.h"
#include "fractionaltypes.h"

void atan_init();
F16 atan_lookup(F15 yi, F15 xi);

#endif
