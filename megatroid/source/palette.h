/******************************\
* 	palette.h
*	By staringmonkey
*	Last modified on 1/07/02
\******************************/

#ifndef PALETTE_H
#define PALETTE_H

/////////////////////////////////////Memory Locations////////////////////////////
u16* BGPaletteMem = (u16*)0x5000000;	//Background Palette
u16* OBJPaletteMem = (u16*)0x5000200;	//Sprite Palette

///////////////////////////////////Function Prototypes////////////////////////////
void LoadBackgroundPalette16(u16* destination, u8 palette);
void LoadBackgroundPalette256(u16* destination);
void LoadSpritePalette16(u16* destination, u8 palette);
void LoadSpritePalette256(u16* destination);
void DarkenBackgroundPalette(void);
void LightenBackgroundPalette(void);
void DarkenSpritePalette(void);
void LightenSpritePalette(void);
void ClearBackgroundPalette(void);
void ClearSpritePalette(void);
void RotatePaletteUp(void);
void RotatePaletteDown(void);
void IsolateRedChannel(void);
void IsolateGreenChannel(void);
void IsolateBlueChannel(void);
void GreyScalePalette(void);
void RandomPalette(void);
void NegativePalette(void);
void SolarizePalette(u16 solThreshold);
void RainbowPalette(u16 rainbowThreshold);
void BWPalette(u16 bwThreshold);

/////////////////////////////////////LoadPalette/////////////////////////////////
void LoadBackgroundPalette16(u16* destination, u8 palette)
{
	//Looping variable
	u16 i;

	//Load the palette into palette memory		
	for(i = 0; i < 16; i++)
	{
		BGPaletteMem[i + palette*16] = destination[i];
	}
}

/////////////////////////////////////LoadPalette/////////////////////////////////
void LoadBackgroundPalette256(u16* destination)
{
	//Looping variable
	u16 i;

	//Load the palette into palette memory		
	for(i = 0; i < 256; i++)
	{
		BGPaletteMem[i] = destination[i];
	}
}

/////////////////////////////////////LoadPalette/////////////////////////////////
void LoadSpritePalette16(u16* destination, u8 palette)
{
	//Looping variable
	u16 i;

	//Load the palette into palette memory		
	for(i = 0; i < 16; i++)
	{
		OBJPaletteMem[i + palette*16] = destination[i];
	}
}

/////////////////////////////////////LoadPalette/////////////////////////////////
void LoadSpritePalette256(u16* destination)
{
	//Looping variable
	u16 i;

	//Load the palette into palette memory		
	for(i = 0; i < 256; i++)
	{
		OBJPaletteMem[i] = destination[i];
	}
}

//////////////////////////////////DarkenBackgroundPalette////////////////////////////////
void DarkenBackgroundPalette(void)
{
	//Looping Variable
	u16 i;

	//Darken the palette
	for(i = 0; i < 256; i++)
	{
		//Keep positive
		if(BGPaletteMem[i] - 1 >= 0)
		{
			BGPaletteMem[i]--;
		}
	}
}

//////////////////////////////////LightenBackgroundPalette////////////////////////////////
void LightenBackgroundPalette(void)
{
	//Looping Variable
	u16 i;

	//Darken the palette
	for(i = 0; i < 256; i++)
	{
		//Keep positive
		if(BGPaletteMem[i] + 1 < 256)
		{
			BGPaletteMem[i]++;
		}
	}
}

//////////////////////////////////DarkenSpritePalette////////////////////////////////
void DarkenSpritePalette(void)
{
	//Looping Variable
	u16 i;

	//Darken the palette
	for(i = 0; i < 256; i++)
	{
		//Keep positive
		if(OBJPaletteMem[i] - 1 >= 0)
		{
			OBJPaletteMem[i]--;
		}
	}
}

//////////////////////////////////LightenSpritePalette////////////////////////////////
void LightenSpritePalette(void)
{
	//Looping Variable
	u16 i;

	//Darken the palette
	for(i = 0; i < 256; i++)
	{
		//Keep positive
		if(OBJPaletteMem[i] + 1 < 256)
		{
			OBJPaletteMem[i]++;
		}
	}
}

//////////////////////////////////ClearBackgroundPalette////////////////////////////////
void ClearBackgroundPalette(void)
{
	//Looping Variable
	u16 i;

	//Darken the palette
	for(i = 0; i < 256; i++)
	{
		BGPaletteMem[i] = 0;
	}
}

////////////////////////////////////ClearSpritePalette////////////////////////////////
void ClearSpritePalette(void)
{
	//Looping Variable
	u16 i;

	//Darken the palette
	for(i = 0; i < 256; i++)
	{
		OBJPaletteMem[i] = 0;
	}
}

////////////////////////////////////RotatePalette////////////////////////////////
void RotatePaletteUp(void)
{
	//Looping Variable
	u16 x;

	//Set 256th color to zeroth color
	BGPaletteMem[256] = BGPaletteMem[0];

	//Rotate the palette
	for(x = 0; x < 256; x++)
	{
		BGPaletteMem[x] = BGPaletteMem[x+1];
	}
}

///////////////////////////////////////RotatePalette////////////////////////////////
void RotatePaletteDown(void)
{
	//Looping Variable
	u16 x;

	//Set 256th color to zeroth color
	BGPaletteMem[0] = BGPaletteMem[256];

	//Rotate the palette
	for(x = 256; x > 0; x--)
	{
		BGPaletteMem[x] = BGPaletteMem[x-1];
	}
}

/////////////////////////////////////IsolateRedChannel//////////////////////////////
void IsolateRedChannel(void)
{
	u16 i;

	//Set Red Channel Only
	for(i = 0; i < 256; i++)
	{
		BGPaletteMem[i] = BGPaletteMem[i] & 31;
	}
}

//////////////////////////////////////IsolateGreenChannel/////////////////////////////
void IsolateGreenChannel(void)
{
	u16 i;

	//Set Red Channel Only
	for(i = 0; i < 256; i++)
	{
		BGPaletteMem[i] = BGPaletteMem[i] & 31<<5;
	}
}

///////////////////////////////////////IsolateBlueChannel////////////////////////////
void IsolateBlueChannel(void)
{
	u16 i;

	//Set Red Channel Only
	for(i = 0; i < 256; i++)
	{
		BGPaletteMem[i] = BGPaletteMem[i] & 31<<10;
	}
}

////////////////////////////////////////GreyScalePalette/////////////////////////////
void GreyScalePalette(void)
{
	//Looping variables
	u16 i,j;

	//Create a greyscale palette
	for(i = 0; i < 32; i++)
	{
		for(j = 0; j < 8; j++)
		{
			BGPaletteMem[i*8+j] = RGB16(i,i,i);
		}
	}
}

//////////////////////////////////////////RandomPalette///////////////////////////////
void RandomPalette(void)
{
	//Looping variables
	u16 i;

	//Random Palette
	for(i = 0; i < 256; i++)
	{
		BGPaletteMem[i] = RGB16(rand() % 31,rand() % 31,rand() % 31);
	}
}

///////////////////////////////////////////NegativePalette///////////////////////////
void NegativePalette(void)
{
	//Looping variables
	u16 i;
	//Color variables
	u8 red,green,blue;

	//Negative Palette
	for(i = 0; i < 256; i++)
	{
		red = BGPaletteMem[i] & 31;
		green = BGPaletteMem[i]>>5 & 31;
		blue = BGPaletteMem[i]>>10 & 31;
		red = 31 - red;
		green = 31 - green;
		blue = 31 - blue;
		BGPaletteMem[i] = red | green<<5 | blue<<10;
	}
}

////////////////////////////////////////SolarizePalette/////////////////////////////
void SolarizePalette(u16 solThreshold)
{
	//Looping variables
	u16 i;
	//Color variables
	u8 red, green, blue;

	//Solarize Palette
	for(i = 0; i < 256; i++)
	{
		//Get current color values
		red = BGPaletteMem[i] & 31;
		green = BGPaletteMem[i]>>5 & 31;
		blue = BGPaletteMem[i]>>10 & 31;

		//Convert them if greater than the threshold value
		if(red > solThreshold)
		{
			red = 31 - red;
		}
		if(green > solThreshold)
		{
			green = 31 - green;
		}
		if(blue > solThreshold)
		{
			blue = 31 - blue;
		}

		//Write back u16o the palette
		BGPaletteMem[i] = red | green<<5 | blue<<10;
	}
}

///////////////////////////////////////RainbowPalette/////////////////////////////
void RainbowPalette(u16 rainbowThreshold)
{
	//Looping variables
	u16 i;
	//Color variables
	u8 red, green, blue;

	//Make the palette solid colors only Palette
	for(i = 0; i < 256; i++)
	{
		//Get current color values
		red = BGPaletteMem[i] & 31;
		green = BGPaletteMem[i]>>5 & 31;
		blue = BGPaletteMem[i]>>10 & 31;

		//Convert them if greater than the threshold value
		if(red > rainbowThreshold)
		{
			red = 31;
		}
		else
		{
			red = 0;
		}
		if(green > rainbowThreshold)
		{
			green = 31;
		}
		else
		{
			green = 0;
		}
		if(blue > rainbowThreshold)
		{
			blue = 31;
		}
		else
		{
			blue = 0;
		}

		//Write back u16o the palette
		BGPaletteMem[i] = red | green<<5 | blue<<10;
	}
}

//////////////////////////////////////BWPalette//////////////////////////////////
void BWPalette(u16 bwThreshold)
{
	//Looping variables
	u16 i;
	//Color variables
	u8 red, green, blue;

	//Make the palette solid colors only Palette
	for(i = 0; i < 256; i++)
	{
		//Get current color values
		red = BGPaletteMem[i] & 31;
		green = BGPaletteMem[i]>>5 & 31;
		blue = BGPaletteMem[i]>>10 & 31;

		//Convert them if greater than the threshold value
		if(red > bwThreshold || green > bwThreshold || blue > bwThreshold)
		{
			red = 31;
			green = 31;
			blue = 31;
		}
		else
		{
			red = 0;
			green = 0;
			blue = 0;
		}

		//Write back u16o the palette
		BGPaletteMem[i] = red | green<<5 | blue<<10;
	}
}

#endif
