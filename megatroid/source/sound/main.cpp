#include "malloc.h"
#include "boyscout.h"

extern const unsigned char tune[];

void WaitForVSync(void)
{
	while(*((volatile unsigned short*)0x04000004) & (1<<0));
	while(!((*((volatile unsigned short*)0x04000004) & (1<<0))));
}

int main(void)
{
	unsigned int nBSSongSize; 

	// Initialize BoyScout
	BoyScoutInitialize();

	// Get needed song memory
	nBSSongSize = BoyScoutGetNeededSongMemory((unsigned char *)tune);

	// Allocate and set BoyScout memory area
	BoyScoutSetMemoryArea((unsigned int)malloc(nBSSongSize));

	// Open song
	BoyScoutOpenSong((unsigned char *)tune);

	BoyScoutMuteChannel1(0);
	BoyScoutMuteChannel2(0);
	BoyScoutMuteChannel3(0);
	BoyScoutMuteChannel4(0);

	// Play song and loop
	BoyScoutPlaySong(1);

	while(1)
	{
		// Wait for vertical blank
		WaitForVSync(); 

		// Update song
		BoyScoutUpdateSong();
	}

	// This part will never be reached but just for completion
	// Free memory
	free((void *)BoyScoutGetMemoryArea()); 
}

