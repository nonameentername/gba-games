/*****************************\
* 	blend.h
*	By staringmonkey
*	Last modified on 12/02/01
\*****************************/

#ifndef BLEND_H
#define BLEND_H

///////////////////////////////Defines///////////////////////////
#define MODE_NONE		0x00
#define MODE_BLEND		BIT00	
#define MODE_FADE_IN	BIT01
#define MODE_FADE_OUT	BIT02

#define TARGET1_BG0		BIT00	
#define TARGET1_BG1		BIT01
#define TARGET1_BG2		BIT02	
#define TARGET1_BG3		BIT03	
#define TARGET1_OBJ		BIT04		
#define TARGET1_BD		BIT05

#define TARGET2_BG0		BIT08	
#define TARGET2_BG1		BIT09	
#define TARGET2_BG2		BIT10	
#define TARGET2_BG3		BIT11	
#define TARGET2_OBJ		BIT12	
#define TARGET2_BD		BIT13
#define TARGET2_ALL		BIT08 | BIT09 | BIT10 | BIT11 | BIT12 | BIT13

#define BlendMode(n)	((n) << 6)	

#define Blend(a, b)		REG_COLEV = (a) | ((b) << 8)
#define Fade(y)			REG_COLEY = (y)

#endif
