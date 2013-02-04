/*****************************\
* 	sound.h
*	By staringmonkey
*	Last modified on 1/22/02
\*****************************/

#ifndef SOUND_H
#define SOUND_H

//////////////////////////Necessary Includes/////////////////////////////
#include "dma.h"

//////////////////////////////Definitions/////////////////////////////////
//SGCNT0_L
#define RIGHT_VOLUME(n)					n
#define LEFT_VOLUME(n)					n<<4
#define SOUND_1_RIGHT_CHANNEL			BIT08
#define SOUND_2_RIGHT_CHANNEL			BIT09
#define SOUND_3_RIGHT_CHANNEL			BIT10
#define SOUND_4_RIGHT_CHANNEL			BIT11
#define SOUND_1_LEFT_CHANNEL			BIT12
#define SOUND_2_LEFT_CHANNEL			BIT13
#define SOUND_3_LEFT_CHANNEL			BIT14
#define SOUND_4_LEFT_CHANNEL			BIT15

//SGCNT0_H
#define SOUND_OUTPUT_1_4				0
#define SOUND_OUTPUT_1_2				BIT00
#define SOUND_OUTPUT_FULL				BIT01
#define DSOUND_A_OUTPUT_HALF			0
#define DSOUND_A_OUTPUT_FULL			BIT02
#define DSOUND_B_OUTPUT_HALF			0
#define DSOUND_B_OUTPUT_FULL			BIT03
#define DSOUND_A_RIGHT_CHANNEL			BIT08
#define DSOUND_A_LEFT_CHANNEL			BIT09
#define DSOUND_A_TIMER_0				0
#define DSOUND_A_TIMER_1				BIT10
#define DSOUND_A_FIFO_RESET				BIT11
#define DSOUND_B_RIGHT_CHANNEL			BIT12
#define DSOUND_B_LEFT_CHANNEL			BIT13
#define DSOUND_B_TIMER_0				0
#define DSOUND_B_TIMER_1				BIT14
#define DSOUND_B_FIFO_RESET				BIT15

//SGCNT1
#define SOUND_1_ENABLE					BIT00
#define SOUND_2_ENABLE					BIT01
#define SOUND_3_ENABLE					BIT02
#define SOUND_4_ENABLE					BIT03
#define SOUND_MASTER_ENABLE				BIT07

//SG10_L
#define S10L_NUM_SWEEP_SHIFTS(n)		n
#define S10L_SWEEP_INCREASE				0
#define S10L_SWEEP_DECREASE				BIT03
#define S10L_SWEEP_CHANGE_FREQ(n)		n<<4

//SG10_H
#define S10H_SOUND_LENGTH(n)			n
#define S10H_WAVEFORM_CYCLE_12			0
#define S10H_WAVEFORM_CYCLE_25			BIT06
#define S10H_WAVEFORM_CYCLE_50			BIT07
#define S10H_WAVEFROM_CYCLE_75			BIT06 | BIT07
#define S10H_ENVELOPE_STEPS(n)			n<<8
#define S10H_ENVELOPE_INCREASE			0
#define S10H_ENVELOPE_DECREASE			BIT11
#define S10H_ENVELOPE_START_VALUE(n)	n<<12

//SG20
#define S20_SOUND_LENGTH(n)				n
#define S20_WAVEFORM_CYCLE_12			0
#define S20_WAVEFORM_CYCLE_25			BIT06
#define S20_WAVEFORM_CYCLE_50			BIT07
#define S20_WAVEFROM_CYCLE_75			BIT06 | BIT07
#define S20_ENVELOPE_STEPS(n)			n<<8
#define S20_ENVELOPE_INCREASE			0
#define S20_ENVELOPE_DECREASE			BIT11
#define S20_ENVELOPE_START_VALUE(n)		n<<12

//SG21
#define S21_FREQUENCY(n)				n
#define S21_CONTINOUS					0
#define S21_COUNTER						BIT14
#define S21_RESTART						BIT15

//SG30_L
#define S30L_32_STEP_WAVEFORM			0
#define S30L_64_STEP_WAVEFORM			BIT05
#define S30L_RAM_BANK_0					0
#define S30L_RAM_BANK_1					BIT06
#define S30L_ENABLE						BIT07

//SG30_H
#define S30H_SOUND_LENGTH(n)			n
#define S30H_OUTPUT_MUTE				0
#define S30H_OUTPUT_FULL				BIT13
#define S30H_OUTPUT_1_2					BIT14
#define S30H_OUTPUT_1_4					BIT13 | BIT14
#define S30H_OUTPUT_3_4					BIT15

//SG31
#define S31_FREQUENCY(n)				n
#define S31_CONTINOUS					0
#define S31_COUNTER						BIT14
#define S31_RESTART						BIT15

//SG40
#define S40_SOUND_LENGTH(n)				n
#define S40_ENVELOPE_STEPS(n)			n<<8
#define S40_ENVELOPE_INCREASE			0
#define S40_ENVELOPE_DECREASE			BIT11
#define S40_ENVELOPE_START_VALUE(n)		n<<12

//SG41
#define S41_DIVIDE_FREQ_2				0
#define S41_DIVIDE_FREQ_1				BIT00
#define S41_DIVIDE_FREQ_1_2				BIT01
#define S41_DIVIDE_FREQ_1_3				BIT00 | BIT01
#define S41_DIVIDE_FREQ_1_4				BIT02
#define S41_DIVIDE_FREQ_1_5				BIT00 | BIT02
#define S41_DIVIDE_FREQ_1_6				BIT01 | BIT02
#define S41_DIVIDE_FREQ_1_7				BIT00 | BIT01 | BIT02
#define S41_POLY_STEPS_15				0
#define S41_POLY_STEPS_7				BIT03
#define S41_POLY_SHIFT_FREQ(n)			n<<4
#define S41_CONTINOUS					0
#define S41_COUNTER						BIT14
#define S41_RESTART						BIT15


////////////////////////////////Function Prototypes//////////////////////////////
void PlayDirectSoundA(u8 *sound, u16 sampleRate, u32 length);
void PlayDirectSoundB(u8 *sound, u16 sampleRate, u32 length);
void UpdateDirectSoundA(void);
void UpdateDirectSoundB(void);

/////////////////////////////////////Globals/////////////////////////////////////
u8 *soundA;				//Sound A sample
u32 soundALength;		//Sound A sample length
u8 soundALoop;			//Sound A loop parameter
u32 soundACurrent;		//Sound A current location
u16 soundASampleRate;	//Sound A sample rate
u8 soundAPlaying;		//Sound A status

u8 *soundB;				//Sound B sample
u32 soundBLength;		//Sound B sample length
u8 soundBLoop;			//Sound B loop parameter
u32 soundBCurrent;		//Sound B current location
u16 soundBSampleRate;	//Sound B sample rate
u8 soundBPlaying;		//Sound B status

////////////////////////////////////PlayDirectSoundA/////////////////////////////////////
void PlayDirectSoundA(u8 *sound, u16 sampleRate, u32 length)
{
	//Stop any previous sample
	soundAPlaying = 0;
	REG_TM0CNT = 0;
	REG_TM0D = 0;
	REG_DMA1SAD = 0;
	REG_DMA1CNT_H = 0;
	REG_DMA1DAD = 0;

	//Output DirectSound A to right channel
	REG_SGCNT0_H |= DSOUND_A_RIGHT_CHANNEL | DSOUND_A_LEFT_CHANNEL | DSOUND_A_FIFO_RESET | DSOUND_A_OUTPUT_FULL;

	//Enable all sound
	REG_SGCNT1 |= SOUND_MASTER_ENABLE;

	//DMA1 Source Addresss
	REG_DMA1SAD = (u32)sound;

	//Set sound A's current sound
	soundA = sound;

	//Set the length, looping, etc
	soundALength = length;
	soundACurrent = 0;
	soundASampleRate = sampleRate;

	//DMA1 Destination Address (REG_SGFIFOA)
	REG_DMA1DAD = 0x40000A0;

	//Write 32 bits into 0x040000A0 (REG_SGFIF0A) every VSync
	REG_DMA1CNT_H =  DMA_DEST_FIXED | DMA_REPEAT | DMA_32 | DMA_TIMING_SYNC_TO_DISPLAY | DMA_ENABLE;

	//Sample Rate
	REG_TM0D = 65536 - (16777216/sampleRate);

	//Enable the timer
	REG_TM0CNT = TIMER_ENABLE | TIMER_IRQ;

	//The sound is playing
	soundAPlaying = 1;
}

////////////////////////////////////PlayDirectSoundB/////////////////////////////////////
void PlayDirectSoundB(u8 *sound, u16 sampleRate, u32 length)
{
	//Stop any previous sample
	soundBPlaying = 0;
	REG_TM1CNT = 0;
	REG_TM1D = 0;
	REG_DMA2SAD = 0;
	REG_DMA2CNT_H = 0;
	REG_DMA2DAD = 0;

	//Output DirectSound A to right channel
	REG_SGCNT0_H |= DSOUND_B_RIGHT_CHANNEL | DSOUND_B_LEFT_CHANNEL | DSOUND_B_FIFO_RESET | DSOUND_B_OUTPUT_FULL;

	//Enable all sound
	REG_SGCNT1 |= SOUND_MASTER_ENABLE;

	//DMA1 Source Addresss
	REG_DMA2SAD = (u32)sound;

	//Set sound B's current sound
	soundB = sound;

	//Set the length and looping
	soundBLength = length;
	soundBCurrent = 0;
	soundBSampleRate = sampleRate;
	soundBPlaying = 1;

	//DMA1 Destination Address (REG_SGFIFOB)
	REG_DMA2DAD = 0x40000A4;

	//Write 32 bits into 0x040000A4 (REG_SGFIF0B) every VSync
	REG_DMA2CNT_H =  DMA_DEST_FIXED | DMA_REPEAT | DMA_32 | DMA_TIMING_SYNC_TO_DISPLAY | DMA_ENABLE;

	//Sample Rate
	REG_TM1D = 65536 - (16777216/sampleRate);

	//Enable the timer
	REG_TM1CNT = TIMER_ENABLE | TIMER_IRQ;
}

/////////////////////////////////////UpdateDirectSoundA////////////////////////////////
void UpdateDirectSoundA(void)
{
	//Increase the sample count
	soundACurrent += 1;

	//See if were finished with the sample
	if(soundACurrent >= soundALength)
	{
		//Stop the sample
		soundAPlaying = 0;
		REG_TM0CNT = 0;
		REG_TM0D = 0;
		REG_DMA1SAD = 0;
		REG_DMA1CNT_H = 0;
		REG_DMA1DAD = 0;
	}
}

/////////////////////////////////////UpdateDirectSoundB////////////////////////////////
void UpdateDirectSoundB(void)
{
	//Increase the sample count
	soundBCurrent += 1;

	//See if were finished with the sample
	if(soundBCurrent >= soundBLength)
	{
		//Stop the sample
		soundBPlaying = 0;
		REG_TM1CNT = 0;
		REG_TM1D = 0;
		REG_DMA2SAD = 0;
		REG_DMA2CNT_H = 0;
		REG_DMA2DAD = 0;
	}
}

#endif
