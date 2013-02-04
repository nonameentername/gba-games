//////////////////////////////////////////////////////////////////////////
// File: BoyScout.h                                                     //
//                                                                      //
// Description: Header file for BoyScout music playback on the GameBoy. //
//                                                                      //
// Author: Christer Andersson (c) 2001                                  //
// Addon:  Willem Kokke                                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// New section of functions were written by Willem Kokke.   //
// He also fixed a silly miss in the wave RAM copy of the   //
// v.0.93 playback libraries. Thanks alot mate :O)          //
//                                                          //
// / Christer                                               //
//////////////////////////////////////////////////////////////

// Make sure header is only included once
#ifndef HEADER_BOYSCOUT_H
#define HEADER_BOYSCOUT_H

// DEFINES ////////

#define PATTERN_PARAMETER_EMPTY	10000

#define SOUND_CHANNEL_COUNT		4

#define SOUND1_PARAMETER_COUNT	7
#define SOUND2_PARAMETER_COUNT	5
#define SOUND3_PARAMETER_COUNT	4
#define SOUND4_PARAMETER_COUNT	7

// Play states
#define PLAYSTATE_STOP		1
#define PLAYSTATE_PLAY		2
#define PLAYSTATE_LOOP		4

////////////////////
// DMA3 Registers //
////////////////////

#define DMA3SRC		*(volatile unsigned int*)0x040000D4   // Source address of transfer (27-bit)
#define DMA3DST		*(volatile unsigned int*)0x040000D8   // Destination address of transfer (27-bit) 
#define DMA3COUNT	*(volatile unsigned short*)0x040000DC // Count of characters that should be transferred (14-bit)
#define DMA3CNT		*(volatile unsigned short*)0x040000DE // DMA3 transfer control register

// Control of destination address for transfer
#define DMACNTDSTINC		(0<<5)	// Increment
#define DMACNTDSTDEC		(1<<5)	// Decrement
#define DMACNTDSTFIX		(2<<5)	// Stay fixed
#define DMACNTDSTINCRELOAD	(3<<5)	// Address is incremented but reset after transfer

// Control of source address for transfer
#define DMACNTSRCINC		(0<<7)	// Increment
#define DMACNTSRCDEC		(1<<7)	// Decrement
#define DMACNTSRCFIX		(2<<7)	// Stay fixed

// If time of transfer is the next VB or HB, DMA transfer
// happens every VB/HB
#define DMACNTREPEAT		(1<<9)

// Tranfer mode
#define DMACNTTRANSFER16	(0<<10)		// Transfer in 16-bit units
#define DMACNTTRANSFER32	(1<<10)		// Transfer in 32-bit units

// Time of transfer
#define DMACNTSTARTIM		(0<<12)		// Immediately
#define DMACNTSTARTVB		(1<<12)		// Next vertical blank
#define DMACNTSTARTHB		(2<<12)		// Next horizontal blank

// Enable interrupt request when transfer is done
#define DMACNTINTENABLE		(1<<14)

// Enable DMA transfer
#define DMACNTENABLE		(1<<15)

// STRUCTS ////////

// Iterator for traversing compressed data
typedef struct SRLEIterator
{
	unsigned char *pData;
	unsigned char iValue;
	unsigned char mask;
	unsigned char cValue;
	short nValue;
	unsigned char iValuePos;
} SRLEIterator;

// Sound 1 pattern
typedef struct
{
	unsigned short nLength;
	unsigned char *apParams[SOUND1_PARAMETER_COUNT];
} SSound1Pattern;

// Sound 2 pattern
typedef struct 
{
	unsigned short nLength;
	unsigned char *apParams[SOUND2_PARAMETER_COUNT];
} SSound2Pattern;

// Sound 3 pattern
typedef struct
{
	unsigned short nLength;
	unsigned char *apParams[SOUND3_PARAMETER_COUNT];
} SSound3Pattern;

// Sound 4 pattern
typedef struct
{
	unsigned short nLength;
	unsigned char *apParams[SOUND4_PARAMETER_COUNT];
} SSound4Pattern;

// PROTOTYPES /////

void RLEISet(unsigned char *pData, SRLEIterator *pRLEIterator);
void RLEINext(SRLEIterator *pRLEIterator);

void BoyScoutInitialize();
void BoyScoutOpenSong(unsigned char *pSongData);
void BoyScoutPlaySong(int nLoop);
void BoyScoutStopSong();
unsigned short BoyScoutGetNeededSongMemory(unsigned char *pSongData);
void BoyScoutSetMemoryArea(unsigned int nMemoryAddress);
unsigned int BoyScoutGetMemoryArea();
int BoyScoutUpdateSong();

void DMA3Copy32(unsigned int Src, unsigned int Dst, unsigned short Count);

// WK Set to 1 to use dma, or to 0 to use software copy
// Software copy is the preffered option, it's faster and more stable
#define USE_DMA 0

// WK  some functions to manipulate the speed of the original song
// You directly manipulate the time a beat takes
// a beat takes x/60, where x is the value you give
// increasing this value slows the song down, decreasing speeds it up

// TODO?  move the timing to a timer to get a better control through increades granularity

unsigned char BoyScoutGetNormalSpeed();
unsigned char BoyScoutGetSpeed();

void BoyScoutIncSpeed(unsigned char speed);
void BoyScoutDecSpeed(unsigned char speed);
void BoyScoutSetSpeed(unsigned char speed);


// WK  some functions to mute individual channels
void BoyScoutMuteChannel1(int mute);
void BoyScoutMuteChannel2(int mute);
void BoyScoutMuteChannel3(int mute);
void BoyScoutMuteChannel4(int mute);

#endif






