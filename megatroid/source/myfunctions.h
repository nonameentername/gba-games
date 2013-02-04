#ifndef myfunctions_H
#define myfunctions_H

#define KEY_IS(k)       ( ! ( ( *KEYS ) & k ) )
#define KEY_REL(k)      (   ( ( *KEYS ) & k ) )


//******************************************************************************
void DrawTile(u16 x, u8 y, u16 tile,u16 *ScreenMem, u8 palPos) //,u8 xFlip, u8 yFlip
{
 //u16 flip=0;
 
 //if(xFlip)
 //  flip+=BIT10;
 //if(yFlip)
 //  flip+=BIT11;
   
 ScreenMem[y*32 + x]=tile | palPos<<12;//+flip;	
}
//******************************************************************************
void FadeOut( u32 aWait )
{
	s8 Phase;
	REG_BLDMOD = 0 | 1 | 2 | 4 | 8 | 128 | 64 | 32;
	for( Phase = 0; Phase < 16; Phase++ )
	{
		REG_COLEY = Phase;
		WaitTime(0, aWait );
	}
}

//******************************************************************************
void FadeIn( u32 aWait )
{
	s8 Phase;
	REG_BLDMOD = 0 | 1 | 2 | 4 | 8 | 128 | 64 | 32;
	for( Phase = 0; Phase < 16; Phase++ )
	{
		REG_COLEY = 16-Phase;
		WaitTime (0, aWait );
	}
}
//******************************************************************************
int Random(int min, int max)
{

srand(REG_TM1D);
return rand() %(max-min+1)+min; //return xrand %(max-min+1)+min;

}



#endif
