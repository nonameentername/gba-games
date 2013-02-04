/*****************************\
*	sprite.h
*	Original by Dovoto
*	Modified by staringmonkey
*	Last modified on 12/18/01
\*****************************/

#ifndef SPRITE_H
#define SPRITE_H

///////////////////////////////Necessary Includes//////////////////////////////
#include <math.h>		//Math functions

///////////////////////////////////Defines/////////////////////////////////////
//Attribute0 stuff
#define ROTATION_FLAG 		BIT08
#define SIZE_DOUBLE			BIT09
#define MODE_NORMAL     	0x0
#define MODE_TRANSPERANT	BIT10
#define MODE_WINDOWED		BIT11
#define MOSAIC				BIT12
#define COLOR_16			0x0000
#define COLOR_256			BIT13
#define SQUARE				0x0
#define WIDE				BIT14
#define TALL				BIT15

//Attribute1 stuff
#define ROTDATA(n)			((n) << 9)
#define HORIZONTAL_FLIP		BIT12
#define VERTICAL_FLIP		BIT13
#define SIZE_8				0x0
#define SIZE_16				BIT14
#define SIZE_32				BIT15
#define SIZE_64				BIT14 | BIT15

//Attribute2 stuff
#define PRIORITY(n)			((n) << 10)
#define PALETTE(n)			((n) << 12)

//Math
#define FIXED s32								//Fixed Point variables
#define PI 3.14159								//Pi
#define RADIAN(n) (((float)n)/(float)180*PI)	//Convert radions->degrees

//Sin and Cos LUTs
FIXED SIN[360];		//Array for precomputed Sin Table
FIXED COS[360];		//Array for precomputed Cos Table

//////////////////////////////////Structures///////////////////////////////////
typedef struct tagOAMEntry
{
	u16 attribute0;
	u16 attribute1;
	u16 attribute2;
	u16 attribute3;
}OAMEntry,*pOAMEntry;

typedef struct tagRotData
{
		
	u16 filler1[3];
	u16 pa;

	u16 filler2[3];
	u16 pb;	
		
	u16 filler3[3];
	u16 pc;	

	u16 filler4[3];
	u16 pd;
}RotData,*pRotData;

////////////////////////////////////Memory Divisions///////////////////////
u16* OAMMem = (u16*)0x7000000;				//Object Attribute Memory

u16* TiledSpriteMem = (u16*)0x6010000;		//Sprite memory with tiled background
u16* BitmapSpriteMem = (u16*)0x6014000;		//Sprite memory with bitmap backgrounds

//////////////////////////////////////Globals///////////////////////////////
OAMEntry sprites[128];
pRotData rotData = (pRotData)sprites;

//////////////////////////////////Function Prototypes//////////////////////
void CalcAngles(void);
void InitializeSprites(void);
void InitializeLimitedSprites(u8 begin, u8 end);
void MoveSprite(OAMEntry* whatSprite, int x, int y);
void RotateSprite(int rotDataIndex, int angle, FIXED x_scale, FIXED y_scale);
void CopyOAM(void);
void TileMode_LoadSpriteTiles(u16 *source, u16 spriteNum, u16 amount);
void BitmapMode_LoadSpriteTiles(u16 *source, u16 spriteNum, u16 amount);

///////////////////////////CalcAngles/////////////////////////
void CalcAngles(void)
{
	//Looping variable
	int i;

	//Precalculate Sin and Cos tables
	for(i = 0; i < 360; i++)
	{
		SIN[i] = (FIXED)(sin(RADIAN(i)) * 256);
		COS[i] = (FIXED)(cos(RADIAN(i)) * 256);
	}
}

/////////////////////////////////////InitializeSprites////////////////////////
void InitializeSprites(void)
{
	//Looping variable
	int i;

	//i through all sprites
	for(i = 0; i < 128; i++)
	{
		sprites[i].attribute0 = 160;  //y > 159, sprite offscreen
		sprites[i].attribute1 = 240;  //x > 239, sprite offscreen
		sprites[i].attribute2 = 0;	 //zero out
	}
}


/////////////////////////////////////InitializeTextSprites////////////////////////
void InitializeLimitedSprites(u8 begin, u8 end)
{
	//Looping variable
	int i;

	//i through all sprites
	for(i = begin; i < end; i++)
	{
		sprites[i].attribute0 = 160;  //y > 159, sprite offscreen
		sprites[i].attribute1 = 240;  //x > 239, sprite offscreen
		sprites[i].attribute2 = 0;	 //zero out
	}
}

/////////////////////////////////////////MoveSprite////////////////////////////
void MoveSprite(OAMEntry* whatSprite, int x, int y)
{
	//Correct if off the left side
	if(x < 0)
	{
		x = 512 + x;
	}

	//Correct if off the right side
	if(y < 0)
	{
		y = 256 + y;
	}

	whatSprite->attribute1 = whatSprite->attribute1 & 0xFE00;	//Clear old X value
	whatSprite->attribute1 = whatSprite->attribute1 | x;		//Set new X value
	
	whatSprite->attribute0 = whatSprite->attribute0 & 0xFF00;	//Clear old Y value
	whatSprite->attribute0 = whatSprite->attribute0 | y;		//Set new Y value
}		

////////////////////////////////////////RotateSprite///////////////////////////
void RotateSprite(int rotDataIndex, int angle, FIXED x_scale,FIXED y_scale)
{
	//Temporary rotation values
	FIXED pa,pb,pc,pd;

	//Do some fixed point math
	pa = ((x_scale) * COS[angle])>>8;
	pb = ((y_scale) * SIN[angle])>>8;
	pc = ((x_scale) * -SIN[angle])>>8;
	pd = ((y_scale) * COS[angle])>>8;

	//Put variables into rotation data structure
	rotData[rotDataIndex].pa = pa;
	rotData[rotDataIndex].pb = pb;
	rotData[rotDataIndex].pc = pc;
	rotData[rotDataIndex].pd = pd;
}

/////////////////////////////////////////CopyOAM/////////////////////////////
void CopyOAM(void)
{
	//Looping variable
	u16 loop;

	//Setup a pointer 
	u16* temp;
	temp = (u16*)sprites;

	//Copy sprite attributes into OAM
	for(loop = 0; loop < 128*4; loop++)
	{
		OAMMem[loop] = temp[loop];
	}
}

///////////////////////////TileMode_LoadSpriteTiles///////////////////////
void TileMode_LoadSpriteTiles(u16 *source, u16 spriteNum, u16 amount)
{
	//Looping variable
	u16 i;

	//Load the tiles into background memory
	for(i = 0; i < amount; i++)
	{
		TiledSpriteMem[i + spriteNum*32] = source[i];
	}
}

/////////////////////////BitmapMode_LoadSpriteTiles///////////////////////
void BitmapMode_LoadSpriteTiles(u16 *source, u16 spriteNum, u16 amount)
{
	//Looping variable
	u16 i;

	//Load the tiles into background memory
	for(i = 0; i < amount; i++)
	{
		BitmapSpriteMem[i + spriteNum*32] = source[i];
	}
}


#endif
