/*****************************\
* 	time.h
*	by staringmonkey
*	Last modified on 12/02/01
\*****************************/

#ifndef TIMERS_H
#define TIMERS_H

/////////////////////////////////////Definitions/////////////////////////////
#define FREQUENCY_0		0x0
#define FREQUENCY_64	BIT00
#define FREQUENCY_256	BIT01
#define FREQUENCY_1024	BIT00 | BIT01

#define TIMER_CASCADE	BIT02
#define TIMER_IRQ		BIT06
#define TIMER_ENABLE	BIT07

////////////////////////////////////Function Prototypes//////////////////////
void WaitBlanks(u16 numBlanks);
void WaitTime(u16 seconds, u16 milliSeconds);
void WaitForVSync(void);

///////////////////////////////////////WaitBlanks/////////////////////////////
void WaitBlanks(u16 numBlanks)
{
	//Looping variables
	u16 i;

	//Wait for however many vSyncs
	while(i < numBlanks)
	{
		WaitForVSync();
		i++;
	}
}

////////////////////////////////////////WaitTime//////////////////////////////
void WaitTime(u16 seconds, u16 milliSeconds)
{
	//Enable timers
	REG_TM2CNT = FREQUENCY_256 | TIMER_ENABLE;
	REG_TM3CNT = TIMER_CASCADE | TIMER_ENABLE;

	//Zero out timer values
	REG_TM2D = 0;
	REG_TM3D = 0;

	//Wait until ? seconds have passed
	while(REG_TM3D < seconds)
	{
	}

	//Zero out base/millisecond timer
	REG_TM2D = 0;

	//1 millisecond = max register value (65536) divided by the # milliseconds in a second (1000)	
	//Wait until ? milliseconds have passed
	while(REG_TM2D / (65536/1000) < milliSeconds)
	{
	}

	//Disable timers
	REG_TM2CNT = 0;
	REG_TM3CNT = 0;
	
	//Zero out timer values
	REG_TM2D = 0;
	REG_TM3D = 0;
}

///////////////////////////////////////WaitForVSync//////////////////////////////
void WaitForVSync(void)
{
	while(*((volatile unsigned short*)0x04000004) & (1<<0));
	while(!((*((volatile unsigned short*)0x04000004) & (1<<0))));

	/*
	__asm 
	{
		mov 	r0, #0x4000006  //0x4000006 is vertical trace counter; when it hits 160					 //160 the vblanc starts
		scanline_wait:	       	//the vertical blank period has begun. done in asm just 
								//because:)
		ldrh	r1, [r0]
		cmp	r1, #160
		bne 	scanline_wait
	}
	*/
}

#endif
