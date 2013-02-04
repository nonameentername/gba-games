/*****************************\
* 	irq.h
*	by staringmonkey
*	Last modified on 12/02/01
\*****************************/

#ifndef IRQ_H
#define IRQ_H

///////////////////Definitions////////////////////
#define IRQ_VBLANK			BIT00
#define IRQ_HBLANK			BIT01
#define IRQ_Y_TRIGGER		BIT02
#define IRQ_TM0				BIT03
#define IRQ_TM1				BIT04
#define IRQ_TM2				BIT05
#define IRQ_TM3				BIT06
#define IRQ_COMMS			BIT07
#define IRQ_DMA0			BIT08
#define IRQ_DMA1			BIT09
#define IRQ_DMA2			BIT10
#define IRQ_DMA3			BIT11
#define IRQ_KEY				BIT12
#define IRQ_CART			BIT13

#define IRQ_ENABLE			BIT00

#endif
