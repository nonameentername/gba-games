/*****************************\
* 	mosiac.h
*	Original by dovoto
*	Modified by staringmonkey
*	Last modified on 12/02/01
\*****************************/

#ifndef MOSIAC_H
#define MOSIAC_H

///////////////////Defines////////////////////
#define MOS_BG_H(x)					(x)
#define MOS_BG_V(x)					(x<<4)
#define MOS_OBJ_H(x)				(x<<8)
#define MOS_OBJ_V(x)				(x<<12)

#define SET_MOSAIC(bh,bv,oh,ov)		REG_MOSAIC = ((bh)+(bv<<4)+(oh<<8)+(ov<<12))

#endif
