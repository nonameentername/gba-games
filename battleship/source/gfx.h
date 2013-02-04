// Gfx.h created 30 Nov 2001
// Desc: Store some of the main graphics routines used by the game


#ifndef GFX_H
#define GFX_H

#include "gba.h"

// GBA stores 15 bit color in blue green red format meaing first 5 
// bits are red, next five green, next blue. What is the 16bit?
#define RGB(r,g,b) ((r)+((g)<<5)+((b)<<10))

// The following code for random was taken from Mines10.zip of gbadev.org, that author
// had the following comment in it:
// random stuff extracted from an email from 'ninge1'
#define RAND_MAX 32767
volatile s32 RAND_RandomData;

void SeedRandom(void);

s32 RAND(s32 Value);

//usage somwhere in program SeedRandom();
//then xrand = RAND(MAX_X); where MAX_X is range upper bound for value you want returned!


#endif

