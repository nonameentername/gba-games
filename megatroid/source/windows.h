/*****************************\
* 	windows.h
*	By staringmonkey
*	Last modified on 12/02/01
\*****************************/

#ifndef WINDOWS_H
#define WINDOWS_H

/////////////////////////////////////Defines/////////////////////////////
#define WIN_RIGHT(n)		(n*8)
#define WIN_LEFT(n)			(n*8)<<8
#define WIN_BOTTOM(n)		(n*8)
#define WIN_TOP(n)			(n*8)<<8

#define WIN0_BG0		BIT00
#define WIN0_BG1		BIT01
#define WIN0_BG2		BIT02
#define WIN0_BG3		BIT03
#define WIN0_SPRITES	BIT04
#define WIN0_BLENDS		BIT05
#define WIN1_BG0		BIT08
#define WIN1_BG1		BIT09
#define WIN1_BG2		BIT10
#define WIN1_BG3		BIT11
#define WIN1_SPRITES	BIT12
#define WIN1_BLENDS		BIT13

#define WINOUT_BG0		BIT00
#define WINOUT_BG1		BIT01
#define WINOUT_BG2		BIT02
#define WINOUT_BG3		BIT03
#define WINOUT_SPRITES	BIT04
#define WINOUT_BLENDS	BIT05
#define SWIN_BG0		BIT08
#define SWIN_BG1		BIT09
#define SWIN_BG2		BIT10
#define SWIN_BG3		BIT11
#define SWIN_SPRITES	BIT12
#define SWIN_BLENDS		BIT13

#endif
