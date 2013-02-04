/*****************************\
* 	saving.h
*	By staringmonkey
*	Last modified on 1/12/02
\*****************************/

#ifndef SAVING_H
#define SAVING_H

/////////////////////////////////Defines///////////////////////////////
#define SRAM 0x0E000000	//32k SRAM = 0x0e000000 - 0x0e00ffff

///////////////////////////////Function Prototypes//////////////////////
void SaveByte(u16 offset, u8 value);
void SaveString(u16 offset, char *string);
void SaveInt(u16 offset, u16 value);
u8 LoadByte(u16 offset);
int LoadInt(u16 offset);
void LoadString(u16 offset, u16 length, char *string);

//////////////////////////////////SaveByte//////////////////////////////
void SaveByte(u16 offset, u8 value)
{
	//Save the value
	*(u8*)(SRAM + offset) = value;
}

//////////////////////////////////SaveString/////////////////////////////
void SaveString(u16 offset, char *string)
{
	//Looping variable
	u16 i;

	//Loop through until hit string end (NULL)
	for(i = 0; i < 32768; i++)
	{
		if(string[i] == NULL)
		{
			break;
		}

		*(u8*)(SRAM + offset + i) = string[i];
	}
}

///////////////////////////////////////SaveInt/////////////////////////////
void SaveInt(u16 offset, u16 value)
{
	//Parts of the variable
	u8 hiByte, lowByte;

	hiByte = value>>8;
	lowByte = (value<<8)>>8;
	
	*(u8*)(SRAM + offset) = hiByte;
	*(u8*)(SRAM + offset + 1) = lowByte;
}

///////////////////////////////////////LoadByte/////////////////////////////
u8 LoadByte(u16 offset)
{
	//Read the value
	return *(u8*)(SRAM + offset);
}

/////////////////////////////////////////LoadInt/////////////////////////////
int LoadInt(u16 offset)
{
	//Parts of the variable
	u8 hiByte, lowByte;

	hiByte = *(u8*)(SRAM + offset);
	lowByte = *(u8*)(SRAM + offset + 1);

	return ((hiByte<<8) | lowByte);
}

//////////////////////////////////////////LoadString///////////////////////////
void LoadString(u16 offset, u16 length, char *string)
{
	//Looping variable
	u16 i;

	for(i = 0; i != length; i++)
	{
		string[i] = *(u8*)(SRAM + offset + i);
	}
}

#endif
