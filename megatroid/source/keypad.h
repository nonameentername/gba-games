/*****************************\
* 	keypad.h
*	Original by Dovoto
*	Modified by staringmonkey
*	Last modified on 12/02/01
\*****************************/

#ifndef KEYPAD_H
#define KEYPAD_H

////////////////////////////Necessary Includes///////////////////////////
#include "time.h"

////////////////////////////////Defines//////////////////////////////
#define KEY_A 			BIT00
#define KEY_B 			BIT01
#define KEY_SELECT		BIT02
#define KEY_START 		BIT03
#define KEY_RIGHT 		BIT04
#define KEY_LEFT 		BIT05
#define KEY_UP 			BIT06
#define KEY_DOWN 		BIT07
#define KEY_R			BIT08
#define KEY_L 			BIT09

///////////////////////////////Globals//////////////////////////////////
volatile u32* KEYS = (volatile u32*)0x04000130;

////////////////////////////Functions Prototypes//////////////////////////
void WaitKey(int whatKey);
void WaitTimeOrKey(int whatKey, int seconds, int milliSeconds);

////////////////////////////////WaitKey/////////////////////////////////
void WaitKey(int whatKey)
{
	//Enable timers
	REG_TM2CNT = FREQUENCY_256 | TIMER_ENABLE;
	REG_TM3CNT = TIMER_CASCADE | TIMER_ENABLE;

	while(1)
	{
		if(!(*KEYS & whatKey))
		{
			break;
		}
	}

	//Seed random number generator (cant hurt, even if that wasnt why we called it)
	srand(REG_TM2D);

	//Disable timers
	REG_TM2CNT = 0;
	REG_TM3CNT = 0;

	//Zero-out timer counters
	REG_TM2D = 0;
	REG_TM3D = 0;
}


//////////////////////////////////WaitTimeOrKey///////////////////////////////
void WaitTimeOrKey(int whatKey, int seconds, int milliSeconds)
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
		if(!(*KEYS & whatKey))
		{
			//Disable timers
			REG_TM2CNT = 0;
			REG_TM3CNT = 0;
			
			//Zero out timer values
			REG_TM2D = 0;
			REG_TM3D = 0;

			//Exit out of the function
			return;
		}
	}

	//Zero out base/millisecond timer
	REG_TM2D = 0;

	//1 millisecond = max register value (65536) divided by the # milliseconds in a second (1000)	
	//Wait until ? milliseconds have passed
	while(REG_TM2D / (65536/1000) < milliSeconds)
	{
		if(!(*KEYS & whatKey))
		{
			//Disable timers
			REG_TM2CNT = 0;
			REG_TM3CNT = 0;
			
			//Zero out timer values
			REG_TM2D = 0;
			REG_TM3D = 0;

			//Exit out of the function
			return;
		}
	}

	//Disable timers
	REG_TM2CNT = 0;
	REG_TM3CNT = 0;
	
	//Zero out timer values
	REG_TM2D = 0;
	REG_TM3D = 0;
}

#endif
