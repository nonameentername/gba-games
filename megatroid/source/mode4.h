/******************************\
* 	mode4.h
*	By staringmonkey
*	Last modified on 12/18/01
\******************************/

#ifndef MODE4_H
#define MODE4_H

//////////////////////////////////////Includes//////////////////////////////
#include <math.h>
#include "dma.h"
#include "screenmode.h"

/////////////////////////////////////Defines///////////////////////////////
#define FrontBuffer (u16*)0x6000000
#define BackBuffer (u16*)0x600A000

///////////////////////////////Function Prototypes////////////////////////
void Flip(void);
void ClearBuffer(void);

////////////////////////////////////////Flip///////////////////////////////////////////
void Flip(void)
{
	//If in back buffer hidden
	if(REG_DISPCNT & BACKBUFFER)
	{
		//Switch to front buffer hidden
		REG_DISPCNT &= ~BACKBUFFER;
		VideoBuffer = BackBuffer;
	}
	//Otherwise front buffer is hidden
	else
	{
		//Switch to back buffer hidden
		REG_DISPCNT |= BACKBUFFER;
		VideoBuffer = FrontBuffer;
	}
}

///////////////////////////////////////ClearBuffer////////////////////////////////////
void ClearBuffer(void)
{
	//If in back buffer
	if(REG_DISPCNT & BACKBUFFER)
	{
		REG_DMA3SAD = 0x08010000;								//Source Address - Some unused VRAM
		REG_DMA3DAD = 0x0600A000;								//Destination Address - Back buffer
		REG_DMA3CNT = DMA_ENABLE | DMA_SOURCE_FIXED | 9600;		//9600 Bytes
	}
	//Otherwise
	else
	{
		REG_DMA3SAD = 0x08010000;								//Source Address - Some unused VRAM
		REG_DMA3DAD = 0x06000000;								//Destination Address - Front buffer
		REG_DMA3CNT = DMA_ENABLE | DMA_SOURCE_FIXED | 9600;		//9600 Bytes
	}
}

#endif
