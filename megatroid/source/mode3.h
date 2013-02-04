/******************************\
* 	mode3.h
*	By staringmonkey
*	Last modified on 12/18/01
\******************************/

#ifndef MODE3_H
#define MODE3_H

//////////////////////////////////Includes//////////////////////////////
#include <math.h>
#include "screenmode.h"

///////////////////////////////////Globals//////////////////////////////
#define SCREENWIDTH		240
#define SCREENHEIGHT	160

///////////////////////////////////Macros////////////////////////////////
#define RGB16(r,g,b)  ((r)+(g<<5)+(b<<10))		//Converts a RGB color to 15 bit RGB	
#define BGR(b,g,r) ((r)+((g)<<5)+((b)<<10))		//Converts a BGR color to 15 bit RGB

///////////////////////////////Function Prototypes////////////////////////
void FillBoxRGB(u16 startX, u16 startY, u16 stopX, u16 stopY, u8 r, u8 g, u8 b);
void FillBox(u16 startX, u16 startY, u16 stopX, u16 stopY, u16 color);
void FillScreenRGB(u8 r, u8 g, u8 b);
void FillScreen(u16 color);
void PlotPixelRGB(u16 x, u16 y, u8 r, u8 g, u8 b);
void PlotPixel(u16 x, u16 y, u16 color);
void LineRGB(u16 x1, u16 y1, u16 x2, u16 y2, u8 r, u8 g, u8 b);
void Line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void CircleRGB(u16 xCenter, u16 yCenter, u16 radius, u8 r, u8 g, u8 b);
void Circle(u16 xCenter, u16 yCenter, u16 radius, u16 color);

//////////////////////////////FillBoxRGB//////////////////////////////////
void FillBoxRGB(u16 startX, u16 startY, u16 stopX, u16 stopY, u8 r, u8 g, u8 b)
{
	//Looping variables
	u16 x;
	u16	y;

	//Loop through the x's
	for(x = startX; x < stopX; x++)
	{
		//Loop through the y's
		for(y = startY; y < stopY; y++)
		{
			//Draw a pixel
			VideoBuffer[y*SCREENWIDTH+x] = RGB16(r,g,b);
		}
	}
}

////////////////////////////////FillBox//////////////////////////////////
void FillBox(u16 startX, u16 startY, u16 stopX, u16 stopY, u16 color)
{
	//Looping variables
	u16 x;
	u16	y;

	//Loop through the x's
	for(x = startX; x < stopX; x++)
	{
		//Loop through the y's
		for(y = startY; y < stopY; y++)
		{
			//Draw a pixel
			VideoBuffer[y*SCREENWIDTH+x] = color;
		}
	}
}

////////////////////////////////FillScreenRGB////////////////////////////
void FillScreenRGB(u8 r, u8 g, u8 b)
{
	//Looping variables
	u16 x;
	u16	y;

	//Loop through the x's
	for(x = 0; x<SCREENWIDTH; x++)
	{
		//Loop through the y's
		for(y = 0; y<SCREENHEIGHT; y++)
		{
			//Draw a pixel
			VideoBuffer[x+y*SCREENWIDTH] = RGB16(r,g,b);
		}
	}
}

/////////////////////////////////FillScreen///////////////////////////////
void FillScreen(u16 color)
{
	//Looping variables
	u16 x;
	u16	y;

	//Loop through the x's
	for(x = 0; x<SCREENWIDTH; x++)
	{
		//Loop through the y's
		for(y = 0; y<SCREENHEIGHT; y++)
		{
			//Draw a pixel
			VideoBuffer[x+y*SCREENWIDTH] = color;
		}
	}
}

//////////////////////////////////PlotPixelRGB/////////////////////////////
void PlotPixelRGB(u16 x, u16 y, u8 r, u8 g, u8 b)
{
	//Draw a pixel
	VideoBuffer[y*SCREENWIDTH+x] = RGB16(r,g,b);
}

////////////////////////////////////PlotPixel//////////////////////////////
void PlotPixel(u16 x, u16 y, u16 color)
{
	//Draw a pixel
	VideoBuffer[y*SCREENWIDTH+x] = color;
}

/////////////////////////////////////LineRGB///////////////////////////////////
//An implementation of the Bresenham Line algorithm
void LineRGB(u16 x1, u16 y1, u16 x2, u16 y2, u8 r, u8 g, u8 b)
{
	//Variables
	u16 i, deltax, deltay, numpixels;
	u16 d, dinc1, dinc2;
	u16 x, xinc1, xinc2;
	u16 y, yinc1, yinc2;

	//Calculate deltaX and deltaY
	deltax = abs(x2 - x1);
	deltay = abs(y2 - y1);

	//Init vars
	if(deltax >= deltay)
	{
		//If x is independent variable
		numpixels = deltax + 1;
		d = (2 * deltay) - deltax;
		dinc1 = deltay << 1;
		dinc2 = (deltay - deltax) << 1;
		xinc1 = 1;
		xinc2 = 1;
		yinc1 = 0;
		yinc2 = 1;
	}
	else
	{
		//If y is independant variable
		numpixels = deltay + 1;
		d = (2 * deltax) - deltay;
		dinc1 = deltax << 1;
		dinc2 = (deltax - deltay) << 1;
		xinc1 = 0;
		xinc2 = 1;
		yinc1 = 1;
		yinc2 = 1;
	}

	//Move the right direction
	if(x1 > x2)
	{
		xinc1 = -xinc1;
		xinc2 = -xinc2;
	}
	if(y1 > y2)
	{
		yinc1 = -yinc1;
		yinc2 = -yinc2;
	}

	x = x1;
	y = y1;

	//Draw the pixels
	for(i = 1; i < numpixels; i++)
	{
		PlotPixelRGB(x, y, r, g, b);

		/*if(d < 0)
		{
			d = d + dinc1;
			x = x + xinc1;
			y = y + yinc1;
		}
		else
		{*/
			d = d + dinc2;
			x = x + xinc2;
			y = y + yinc2;
		//}
	}
}

////////////////////////////////////Line////////////////////////////////
//An implementation of the Bresenham Line algorithm
void Line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	//Variables
	u16 i, deltax, deltay, numpixels;
	u16 d, dinc1, dinc2;
	u16 x, xinc1, xinc2;
	u16 y, yinc1, yinc2;

	//Calculate deltaX and deltaY
	deltax = abs(x2 - x1);
	deltay = abs(y2 - y1);

	//Init vars
	if(deltax >= deltay)
	{
		//If x is independent variable
		numpixels = deltax + 1;
		d = (2 * deltay) - deltax;
		dinc1 = deltay << 1;
		dinc2 = (deltay - deltax) << 1;
		xinc1 = 1;
		xinc2 = 1;
		yinc1 = 0;
		yinc2 = 1;
	}
	else
	{
		//If y is independant variable
		numpixels = deltay + 1;
		d = (2 * deltax) - deltay;
		dinc1 = deltax << 1;
		dinc2 = (deltax - deltay) << 1;
		xinc1 = 0;
		xinc2 = 1;
		yinc1 = 1;
		yinc2 = 1;
	}

	//Move the right direction
	if(x1 > x2)
	{
		xinc1 = -xinc1;
		xinc2 = -xinc2;
	}
	if(y1 > y2)
	{
		yinc1 = -yinc1;
		yinc2 = -yinc2;
	}

	x = x1;
	y = y1;

	//Draw the pixels
	for(i = 1; i < numpixels; i++)
	{
		PlotPixel(x, y, color);

		/*if(d < 0)
		{
			d = d + dinc1;
			x = x + xinc1;
			y = y + yinc1;
		}
		else
		{*/
			d = d + dinc2;
			x = x + xinc2;
			y = y + yinc2;
		//}
	}
}

////////////////////////////////////CircleRGB/////////////////////////////////
//An implementation of the Bresenham Circle algorithm
void CircleRGB(u16 xCenter, u16 yCenter, u16 radius, u8 r, u8 g, u8 b)
{
	u16 x = 0;
	u16 y = radius;
	u16 p = 3 - 2 * radius;
	while (x <= y)
	{ 
		PlotPixelRGB(xCenter + x, yCenter + y, r, g, b);
		PlotPixelRGB(xCenter - x, yCenter + y, r, g, b);
		PlotPixelRGB(xCenter + x, yCenter - y, r, g, b);
		PlotPixelRGB(xCenter - x, yCenter - y, r, g, b);
		PlotPixelRGB(xCenter + y, yCenter + x, r, g, b);
		PlotPixelRGB(xCenter - y, yCenter + x, r, g, b);
		PlotPixelRGB(xCenter + y, yCenter - x, r, g, b);
		PlotPixelRGB(xCenter - y, yCenter - x, r, g, b);

		/*if (p < 0)
		{
			p += 4 * x++ + 6;
		}
		else
		{*/
			p += 4 * (x++ - y--) + 10;
		//}
	}
}

//////////////////////////////////////Circle//////////////////////////////
//An implementation of the Bresenham Circle algorithm
void Circle(u16 xCenter, u16 yCenter, u16 radius, u16 color)
{
	u16 x = 0;
	u16 y = radius;
	u16 p = 3 - 2 * radius;
	while (x <= y)
	{ 
		PlotPixel(xCenter + x, yCenter + y, color);
		PlotPixel(xCenter - x, yCenter + y, color);
		PlotPixel(xCenter + x, yCenter - y, color);
		PlotPixel(xCenter - x, yCenter - y, color);
		PlotPixel(xCenter + y, yCenter + x, color);
		PlotPixel(xCenter - y, yCenter + x, color);
		PlotPixel(xCenter + y, yCenter - x, color);
		PlotPixel(xCenter - y, yCenter - x, color);

		/*if (p < 0)
		{
			p += 4 * x++ + 6;
		}
		else
		{*/
			p += 4 * (x++ - y--) + 10;
		//}
	}
}

#endif
