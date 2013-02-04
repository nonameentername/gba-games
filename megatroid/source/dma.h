/*****************************\
* 	dma.h
*	Original by dovoto
*	Modified by staringmonkey
*	Last modified on 12/02/01
\*****************************/

#ifndef DMA_H
#define DMA_H

////////////////////////////////Definitions///////////////////////////////////
#define DMA_ENABLE					BIT15
#define DMA_INTERUPT_ENABLE			BIT14
#define DMA_TIMING_IMMEDIATE		0
#define DMA_TIMING_VBLANK			BIT12
#define DMA_TIMING_HBLANK			BIT13
#define DMA_TIMING_SYNC_TO_DISPLAY	BIT12 | BIT13
#define DMA_16						0
#define DMA_32						BIT10
#define DMA_REPEAT					BIT09
#define DMA_SOURCE_INCREMENT		0
#define DMA_SOURCE_DECREMENT		BIT07
#define DMA_SOURCE_FIXED			BIT08
#define DMA_DEST_INCREMENT			0
#define DMA_DEST_DECREMENT			BIT05
#define DMA_DEST_FIXED				BIT06
#define DMA_DEST_RELOAD				BIT05 | BIT06

#endif
