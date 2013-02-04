/*****************************\
* 	gba.h
*	Original by eloist
*	Modified by staringmonkey
*	Last modified on 12/18/01
\*****************************/

#ifndef GBA_H
#define GBA_H

/////////////////////////////////////Type Definitions/////////////////////////////////
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;

typedef unsigned char byte;
typedef unsigned short hword;
typedef unsigned long word;

////////////////////////////////////////////Bits/////////////////////////////////
#define BIT00 1
#define BIT01 2
#define BIT02 4
#define BIT03 8
#define BIT04 16
#define BIT05 32
#define BIT06 64
#define BIT07 128
#define BIT08 256
#define BIT09 512
#define BIT10 1024
#define BIT11 2048
#define BIT12 4096
#define BIT13 8192
#define BIT14 16384
#define BIT15 32768

/////////////////////////////////Register Defines//////////////////////////		
#define REG_INTERUPT	*(volatile u32*)0x3007FFC		//Interrupt Register
#define REG_DISPCNT		*(volatile u32*)0x4000000		//Display Control (Mode)
#define REG_DISPCNT_L	*(volatile u16*)0x4000000		//Display Control (Mode) Lo Word
#define REG_DISPCNT_H	*(volatile u16*)0x4000002		//Display Control (Mode) Hi Word
#define REG_DISPSTAT	*(volatile u16*)0x4000004		//Display Status
#define REG_VCOUNT		*(volatile u16*)0x4000006		//Vertical Control (Sync)
#define REG_BG0CNT		*(volatile u16*)0x4000008		//Background 0
#define REG_BG1CNT		*(volatile u16*)0x400000A		//Background 1
#define REG_BG2CNT		*(volatile u16*)0x400000C		//Background 2
#define REG_BG3CNT		*(volatile u16*)0x400000E		//Background 3
#define REG_BG0HOFS		*(volatile u16*)0x4000010		//Background 0 Horizontal Offset
#define REG_BG0VOFS		*(volatile u16*)0x4000012		//Background 0 Vertical Offset
#define REG_BG1HOFS		*(volatile u16*)0x4000014		//Background 1 Horizontal Offset
#define REG_BG1VOFS		*(volatile u16*)0x4000016		//Background 1 Vertical Offset
#define REG_BG2HOFS		*(volatile u16*)0x4000018		//Background 2 Horizontal Offset
#define REG_BG2VOFS		*(volatile u16*)0x400001A		//Background 2 Vertical Offset
#define REG_BG3HOFS		*(volatile u16*)0x400001C		//Background 3 Horizontal Offset
#define REG_BG3VOFS		*(volatile u16*)0x400001E		//Background 3 Vertical Offset
#define REG_BG2PA		*(volatile u16*)0x4000020		//Background 2 PA Rotation (pa = x_scale * cos(angle);)
#define REG_BG2PB		*(volatile u16*)0x4000022		//Background 2 PB Rotation (pb = y_scale * sin(angle);)
#define REG_BG2PC		*(volatile u16*)0x4000024		//Background 2 PC Rotation (pc = x_scale * -sin(angle);)
#define REG_BG2PD		*(volatile u16*)0x4000026		//Background 2 PD Rotation (pd = y_scale * cos(angle);)
#define REG_BG2X		*(volatile u32*)0x4000028		//Background 2 X Location
#define REG_BG2X_L		*(volatile u16*)0x4000028		//Background 2 X Location Lo Word
#define REG_BG2X_H		*(volatile u16*)0x400002A		//Background 2 X Location Hi Word
#define REG_BG2Y		*(volatile u32*)0x400002C		//Background 2 Y Location
#define REG_BG2Y_L		*(volatile u16*)0x400002C		//Background 2 Y Location Lo Word
#define REG_BG2Y_H		*(volatile u16*)0x400002E		//Background 2 Y Location Hi Word
#define REG_BG3PA		*(volatile u16*)0x4000030		//Background 3 PA Rotation (pa = x_scale * cos(angle);)
#define REG_BG3PB		*(volatile u16*)0x4000032		//Background 3 PB Rotation (pb = y_scale * sin(angle);)
#define REG_BG3PC		*(volatile u16*)0x4000034		//Background 3 PC Rotation (pc = x_scale * -sin(angle);)
#define REG_BG3PD		*(volatile u16*)0x4000036		//Background 3 PD Rotation (pd = y_scale * cos(angle);)
#define REG_BG3X		*(volatile u32*)0x4000038		//Background 3 X Location
#define REG_BG3X_L		*(volatile u16*)0x4000038		//Background 3 X Location Lo Word
#define REG_BG3X_H		*(volatile u16*)0x400003A		//Background 3 X Location Hi Word
#define REG_BG3Y		*(volatile u32*)0x400003C		//Background 3 Y Location
#define REG_BG3Y_L		*(volatile u16*)0x400003C		//Background 3 Y Location Lo Word
#define REG_BG3Y_H		*(volatile u16*)0x400003E		//Background 3 Y Location Hi Word
#define REG_WIN0H		*(volatile u16*)0x4000040		//Window 0 X coords (bits 0-7 right, bits 8-16 left)
#define REG_WIN1H		*(volatile u16*)0x4000042		//Window 1 X coords (bits 0-7 right, bits 8-16 left)
#define REG_WIN0V		*(volatile u16*)0x4000044		//Window 0 Y coords (bits 0-7 bottom, bits 8-16 top)
#define REG_WIN1V		*(volatile u16*)0x4000046		//Window 1 Y coords (bits 0-7 bottom, bits 8-16 top)
#define REG_WININ		*(volatile u16*)0x4000048		//Inside Window Settings
#define REG_WINOUT		*(volatile u16*)0x400004A		//Outside Window Settings
#define REG_MOSAIC		*(volatile u32*)0x400004C		//Mosaic Mode
#define REG_MOSAIC_L	*(volatile u32*)0x400004C		//Mosaic Mode Lo Word
#define REG_MOSAIC_H	*(volatile u32*)0x400004E		//Mosaic Mode Hi Word
#define REG_BLDMOD		*(volatile u16*)0x4000050		//Blend Mode
#define REG_COLEV		*(volatile u16*)0x4000052		//Blending Coeffecients
#define REG_COLEY		*(volatile u16*)0x4000054		//Blending Coeffecients
#define REG_SG10		*(volatile u32*)0x4000060		//Sound 10
#define REG_SG10_L		*(volatile u16*)0x4000060		//Sound 10 Lo Word
#define REG_SG10_H		*(volatile u16*)0x4000062		//Sound 10 Hi Word
#define REG_SG11		*(volatile u16*)0x4000064		//Sound 11
#define REG_SG20		*(volatile u16*)0x4000068		//Sound 20
#define REG_SG21		*(volatile u16*)0x400006C		//Sound 21
#define REG_SG30		*(volatile u32*)0x4000070		//Sound 30
#define REG_SG30_L		*(volatile u16*)0x4000070		//Sound 30 Lo Word
#define REG_SG30_H		*(volatile u16*)0x4000072		//Sound 30 Hi Word
#define REG_SG31		*(volatile u16*)0x4000074		//Sound 31
#define REG_SG40		*(volatile u16*)0x4000078		//Sound 40
#define REG_SG41		*(volatile u16*)0x400007C		//Sound 41
#define REG_SGCNT0		*(volatile u32*)0x4000080		//Sound Control 0
#define REG_SGCNT0_L	*(volatile u16*)0x4000080		//Sound Control 0 Lo Word
#define REG_SGCNT0_H	*(volatile u16*)0x4000082		//Sound Control 0 Hi Word
#define REG_SGCNT1		*(volatile u16*)0x4000084		//Sound Control 1
#define REG_SGBIAS		*(volatile u16*)0x4000088		//???
#define REG_SGWR0		*(volatile u32*)0x4000090		//???
#define REG_SGWR0_L		*(volatile u16*)0x4000090		//???
#define REG_SGWR0_H		*(volatile u16*)0x4000092		//???
#define REG_SGWR1		*(volatile u32*)0x4000094		//???
#define REG_SGWR1_L		*(volatile u16*)0x4000094		//???
#define REG_SGWR1_H		*(volatile u16*)0x4000096		//???
#define REG_SGWR2		*(volatile u32*)0x4000098		//???
#define REG_SGWR2_L		*(volatile u16*)0x4000098		//???
#define REG_SGWR2_H		*(volatile u16*)0x400009A		//???
#define REG_SGWR3		*(volatile u32*)0x400009C		//???
#define REG_SGWR3_L		*(volatile u16*)0x400009C		//???
#define REG_SGWR3_H		*(volatile u16*)0x400009E		//???
#define REG_SGFIF0A		*(volatile u32*)0x40000A0		//DirectSound A FIFO
#define REG_SGFIFOA_L	*(volatile u16*)0x40000A0		//DirectSound A FIFO Lo Word
#define REG_SGFIFOA_H	*(volatile u16*)0x40000A2		//DirectSound A FIFO Hi Word
#define REG_SGFIFOB		*(volatile u32*)0x40000A4		//DirectSound B FIFO
#define REG_SGFIFOB_L	*(volatile u16*)0x40000A4		//DirectSound B FIFO Lo Word
#define REG_SGFIFOB_H	*(volatile u16*)0x40000A6		//DirectSound B FIFO Hi Word
#define REG_DMA0SAD		*(volatile u32*)0x40000B0		//DMA0 Source Address
#define REG_DMA0SAD_L	*(volatile u16*)0x40000B0		//DMA0 Source Address Lo Word
#define REG_DMA0SAD_H	*(volatile u16*)0x40000B2		//DMA0 Source Address Hi Word
#define REG_DMA0DAD		*(volatile u32*)0x40000B4		//DMA0 Destination Address
#define REG_DMA0DAD_L	*(volatile u16*)0x40000B4		//DMA0 Destination Address Lo Word
#define REG_DMA0DAD_H	*(volatile u16*)0x40000B6		//DMA0 Destination Address Hi Word
#define REG_DMA0CNT		*(volatile u32*)0x40000B8		//DMA0 Control (Amount)
#define REG_DMA0CNT_L	*(volatile u16*)0x40000B8		//DMA0 Control Lo Word
#define REG_DMA0CNT_H	*(volatile u16*)0x40000BA		//DMA0 Control Hi Word
#define REG_DMA1SAD		*(volatile u32*)0x40000BC		//DMA1 Source Address
#define REG_DMA1SAD_L	*(volatile u16*)0x40000BC		//DMA1 Source Address Lo Word
#define REG_DMA1SAD_H	*(volatile u16*)0x40000BE		//DMA1 Source Address Hi Word
#define REG_DMA1DAD		*(volatile u32*)0x40000C0		//DMA1 Desination Address
#define REG_DMA1DAD_L	*(volatile u16*)0x40000C0		//DMA1 Destination Address Lo Word
#define REG_DMA1DAD_H	*(volatile u16*)0x40000C2		//DMA1 Destination Address Hi Word
#define REG_DMA1CNT		*(volatile u32*)0x40000C4		//DMA1 Control (Amount)
#define REG_DMA1CNT_L	*(volatile u16*)0x40000C4		//DMA1 Control Lo Word
#define REG_DMA1CNT_H	*(volatile u16*)0x40000C6		//DMA1 Control Hi Word
#define REG_DMA2SAD		*(volatile u32*)0x40000C8		//DMA2 Source Address
#define REG_DMA2SAD_L	*(volatile u16*)0x40000C8		//DMA2 Source Address Lo Word
#define REG_DMA2SAD_H	*(volatile u16*)0x40000CA		//DMA2 Source Address Hi Word
#define REG_DMA2DAD		*(volatile u32*)0x40000CC		//DMA2 Destination Address
#define REG_DMA2DAD_L	*(volatile u16*)0x40000CC		//DMA2 Destination Address Lo Word
#define REG_DMA2DAD_H	*(volatile u16*)0x40000CE		//DMA2 Destination Address Hi Word
#define REG_DMA2CNT		*(volatile u32*)0x40000D0		//DMA2 Control (Amount)
#define REG_DMA2CNT_L	*(volatile u16*)0x40000D0		//DMA2 Control Lo Word
#define REG_DMA2CNT_H	*(volatile u16*)0x40000D2		//DMA2 Control Hi Word
#define REG_DMA3SAD		*(volatile u32*)0x40000D4		//DMA3 Source Address
#define REG_DMA3SAD_L	*(volatile u16*)0x40000D4		//DMA3 Source Address Lo Word
#define REG_DMA3SAD_H	*(volatile u16*)0x40000D6		//DMA3 Source Address Hi Word
#define REG_DMA3DAD		*(volatile u32*)0x40000D8		//DMA3 Destination Address
#define REG_DMA3DAD_L	*(volatile u16*)0x40000D8		//DMA3 Destination Address Lo Word
#define REG_DMA3DAD_H	*(volatile u16*)0x40000DA		//DMA3 Destination Address Hi Word
#define REG_DMA3CNT		*(volatile u32*)0x40000DC		//DMA3 Control (Amount)
#define REG_DMA3CNT_L	*(volatile u16*)0x40000DC		//DMA3 Control Lo Word
#define REG_DMA3CNT_H	*(volatile u16*)0x40000DE		//DMA3 Control Hi Word
#define REG_TM0D		*(volatile u16*)0x4000100		//Timer 1 Data
#define REG_TM0CNT		*(volatile u16*)0x4000102		//Timer 1 Control
#define REG_TM1D		*(volatile u16*)0x4000104		//Timer 2 Data
#define REG_TM1CNT		*(volatile u16*)0x4000106		//Timer 2 Control
#define REG_TM2D		*(volatile u16*)0x4000108		//Timer 3 Data
#define REG_TM2CNT		*(volatile u16*)0x400010A		//Timer 3 Control
#define REG_TM3D		*(volatile u16*)0x400010C		//Timer 4 Data
#define REG_TM3CNT		*(volatile u16*)0x400010E		//Timer 4 Control
#define REG_SCD0		*(volatile u16*)0x4000120		//32-bit Normal Serial Communication Data 0 / Multi-play
#define REG_SCD1		*(volatile u16*)0x4000122		//32-bit Normal Serial Communication Data 1 /Multi-play
#define REG_SCD2		*(volatile u16*)0x4000124		//Multi-play Communication Data 2
#define REG_SCD3		*(volatile u16*)0x4000126		//Multi-play Communication Data 3
#define REG_SCCNT		*(volatile u32*)0x4000128		//???
#define REG_SCCNT_L		*(volatile u16*)0x4000128		//???
#define REG_SCCNT_H		*(volatile u16*)0x400012A		//???
#define REG_P1			*(volatile u16*)0x4000130		//Player 1 Input
#define REG_P1CNT		*(volatile u16*)0x4000132		//Player 1 Input Interrupt Status
#define REG_R			*(volatile u16*)0x4000134		//???
#define REG_HS_CTRL		*(volatile u16*)0x4000140		//???
#define REG_JOYRE		*(volatile u32*)0x4000150		//???
#define REG_JOYRE_L		*(volatile u16*)0x4000150		//???
#define REG_JOYRE_H		*(volatile u16*)0x4000152		//???
#define REG_JOYTR		*(volatile u32*)0x4000154		//???
#define REG_JOYTR_L		*(volatile u16*)0x4000154		//???
#define REG_JOYTR_H		*(volatile u16*)0x4000156		//???
#define REG_JSTAT		*(volatile u32*)0x4000158		//???
#define REG_JSTAT_L		*(volatile u16*)0x4000158		//???
#define REG_JSTAT_H		*(volatile u16*)0x400015A		//???
#define REG_IE			*(volatile u16*)0x4000200		//Master Interrupt Enable
#define REG_IF			*(volatile u16*)0x4000202		//Interrupt Flags
#define REG_WSCNT		*(volatile u16*)0x4000204		//???
#define REG_IME			*(volatile u16*)0x4000208		//Interrupt Master Enable
#define REG_PAUSE		*(volatile u16*)0x4000300		//Pause

#endif
