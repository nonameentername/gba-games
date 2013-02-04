/*****************************\
* 	screenmode.h
*	Original by dovoto
*	Modified by staringmonkey
*	Last modified on 01/26/02
\*****************************/

#ifndef SCREENMODE_H
#define SCREENMODE_H

////////////////////////////////////Defines/////////////////////////////
#define MODE_0			0x0
#define MODE_1			BIT00
#define MODE_2			BIT01
#define MODE_3			BIT00 | BIT01
#define MODE_4			BIT02
#define MODE_5			BIT00 | BIT02

#define BACKBUFFER		BIT04
#define H_BLANK_OAM		BIT05 

#define OBJ_MAP_2D		0x0
#define OBJ_MAP_1D		BIT06

#define FORCE_BLANK		BIT07

#define BG0_ENABLE		BIT08
#define BG1_ENABLE		BIT09 
#define BG2_ENABLE		BIT10
#define BG3_ENABLE		BIT11
#define OBJ_ENABLE		BIT12 

#define WIN0_ENABLE		BIT13 
#define WIN1_ENABLE		BIT14
#define WINOBJ_ENABLE	BIT15

#define SET_MODE(mode)	REG_DISPCNT = (mode)

/////////////////////////////////Memory	Locations////////////////////////////
u16* VideoBuffer = (u16*)0x6000000;		//Display Memory (the screen)

#endif
