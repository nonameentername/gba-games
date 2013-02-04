#include "gba.h"
#include "screenmode.h"
#include "keypad.h"
#include "gfx.h"
#include "timers.h"
#include "gfx/logo.h"
#include "gfx/tetris.h"
#include "gfx/background.h"
#include "gfx/colors.h"
#include "gfx/numbers.h"
#include "gfx/gameover.h"
#include "gfx/temp.h"
#include "gfx/mike.h"
#include "gfx/letters.h"

# define MAXROW (20)
# define MAXCOL (12)
# define MAXROW_S (4)
# define MAXCOL_S (4)
# define MAX_X    (6)

#define GAMEPAK_RAM  ((u8*)0x0E000000)
u8 *pSaveMemory = GAMEPAK_RAM;

//example to save: pSaveMemory[0] = 1;

// some memory location defines
u16* FrontBuffer = (u16*)0x6000000;
u16* BackBuffer = (u16*)0x600A000;
u16* videoBuffer;
volatile u16* ScanlineCounter = (volatile u16*)0x4000006;

u16* paletteMem = (u16*)0x5000000;	//PalMemory is 256 16 bit BGR values starting at 0x5000000

u8 front [MAXROW][MAXCOL]; //front array used for shape movement
u8 back  [MAXROW][MAXCOL]; //back array used for still shapes
u8 shape [MAXROW_S][MAXCOL_S]; //shape that is moved on the front array
u8 shape2[MAXROW_S][MAXCOL_S]; //next shape to appear.
u8 temp  [MAXROW_S][MAXCOL_S]; //temp shape used to rotate the shape
int tlines=0;  //total lines
int score =0;  //score
int level =1;  //level

//function prototypes

// Mode 4 is 240(120)x160 by 8bit
void PlotPixel(int x,int y, unsigned short int c)
{
	videoBuffer[(y) *120 + (x)] = (c);
}

// Mode 4 is 240(120)x160 by 8bit
void PlotPixelTransparent(int x,int y, unsigned short int c)
{
	unsigned short int temp;
	if ((c & 0x00FF) == 0) // bottom is transparent
	{
		if ((c & 0xFF00) == 0) // top is also transparent
			return;
		// so bottom transparent, top is not so....
		temp = ((videoBuffer[(y) * 120 + (x)]) & 0x00FF);
		temp |= c; // or it with c (to get the highpart of c in
		videoBuffer[(y) *120 + (x)] = (temp);
	}
	else
	if 	((c & 0xFF00) == 0) // only the top is transprent
	{
		temp = ((videoBuffer[(y) * 120 + (x)]) & 0xFF00);
		temp |= c;
		videoBuffer[(y) *120 + (x)] = (temp);
	}
	else // no part is transparent
		videoBuffer[(y) *120 + (x)] = (c);
}

void Sleep(int i);
void EraseScreen(void);		// erases the screen in mode4
void DoIntro(void);      //does the intro
void DoIntro2(void);     //does the rest of the intro.
void WaitForStart(void);	// waits for the start key to be pressed
void Flip(void);			// flips between the back front buffer
void WaitForVblank(void);	// waits for the drawer to get to end before flip
void background(void);  //draws the background of tetris
void FadeOut( u32 aWait ); //fades out
void FadeIn( u32 aWait ); // fades in
void print(void);  //prints out the figures and the score
void shaper(int type);  //creates a random number and calls makeshape
void makeshape(int a,int b,int c, int d, int e, int f, int g, int h, int i,
int j, int k,int l, int m, int n, int o, int p, int type);  //creates a shape;
void change(int fx, int fy); //moves the shape on the grid
void move(void);   //spins the figure around
void clearfront(void);  //clears the front array
int check(int g,int h); //checks to see if the figure can go in the desired area
void combine(void); //moves the shape into the back array
void clearline(void); //clears the lines
void movedown(int j); //moves the squares down if the figures are cleared
void numbers(int numx, int numy,int num); //prints out the numbers
void numbers1(int numx, int numy,int num); //prints out the numbers white black background
void gameover(void); //prints gameover on the screen
void colors(int numx, int numy,int num); //prints out the different shapes
void save(void); //saves the highscore
void letters(int numx, int numy, char num); //used to print out the letters
void writetext(int x, int y, char *message); //used to print out the letters
void writetextslow(int x, int y, char *message); //used to print out the letters
void writetextblock(char *message, int sleepamount); //prints out letters onto the whole screen;
void writetextmemory(int x, int y, char *message);
void writenumbers(int x, int y, int num); // used to print out numbers
void writenumbersslow(int x, int y, int num); // used to print out numbers
int pow(int num, int exp);  //raises a number to a certain power... made to work with my program
void memory(void);  //checks to see if there is a save if not makes one
void nameinput(char namesent[]); // imput your name
void choices(void);

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
int main (void)
{
  int x,y,a;
  int end;
  int count=0;
  int fin=0;

  int fx, fy;

  //start timer
  REG_TM3CNT = TIME_FREQUENCY_1024 | TIME_ENABLE;
  //zero the timer
  REG_TM3D = 0;

  SetMode(MODE_4 | BG2_ENABLE );   //set to mode 4

  //erase memory;
  //writetextmemory(200,0,"    _");

 while(1)
 {
   fin=0;
   count=0;
   
   memory();
   //nameinput();
   //save();

   for (y=0; y<=16; y++)
    for (x=0; x<=10; x++)
      {
      front [y][x]=0;
      back  [y][x]=0;
      }

    for (a=0;a<=17;++a)
      {
      back[a][0]=1;

      back[a][11]=1;

      if (a<=11)
         {
          back[17][a]=1;
         }
      }

	EraseScreen();
   WaitForVblank();
   choices();
	DoIntro();
   writetextblock(" A long time ago in a galaxy  far far away there was..._", 100);
   DoIntro2();
   background();

   EraseScreen();
   WaitForVblank();
   EraseScreen();
   WaitForVblank();
   FadeIn(0);

   while (fin!=1)
   {

   if((back[3][4]!=0)||(back[3][5]!=0)||(back[3][6]!=0)||(back[3][7]!=0))  //checking for gameover
    {
    gameover();
    save();
    EraseScreen();
    WaitForVblank();
    EraseScreen();
    WaitForVblank();
    score=0;
    level=0;
    tlines=0;
    fin=1;
    }

   fx=4;
   fy=0;
   end=0;

   if(count==0)
   {
   shaper(1);
   Sleep(1);
   shaper(2);
   }
   else
   {
   for (x=1; x<=4; x++)
     for(y=1; y<=4; y++)
       shape[x][y]=shape2[x][y];
       shaper(2);
   }

   print();

   while (end!=1)
    {
     if (! ((*KEYS) & KEY_RIGHT) )
       {
        Sleep(2);
         switch(check(fx+1,fy))
           {
           case 0: change(fx,fy);
              break;
           case 1:fx=fx+1;
                change(fx,fy);
              break;
            }

       }

     if (! ((*KEYS) & KEY_LEFT) )
       {
        Sleep(2);
           switch(check(fx-1,fy))
           {
           case 0: change(fx,fy);
              break;
           case 1:fx=fx-1;
                change(fx,fy);
              break;
            }
       }

       if (! ((*KEYS) & KEY_B) )
       {
          Sleep(5);
           move();
           switch(check(fx,fy))
           {
           case 0: move(); move(); move(); change(fx,fy);
              break;
           case 1:
                change(fx,fy);
              break;
            }
       }

       if (! ((*KEYS) & KEY_A) )
       {
          Sleep(5);
           move(); move();move();
           switch(check(fx,fy))
           {
           case 0: move(); change(fx,fy);
              break;
           case 1:
                change(fx,fy);
              break;
            }
       }


     if (! ((*KEYS) & KEY_DOWN) )
       {
           switch(check(fx,fy+1))
           {
           case 0:change(fx,fy); end=1;
              break;
           case 1:fy=fy+1;
                change(fx,fy);
              break;
            }
       }

     if (! ((*KEYS) & KEY_START) )
       {
         EraseScreen();
         WaitForVblank();
         EraseScreen();
         WaitForVblank();
         Sleep(10);
         writetextslow(45,65,"-PAUSED-_");
         while( ((*KEYS) & KEY_START) )
          {
          Sleep(1);
          }
          Sleep(5);
       }

     if ((! ((*KEYS) & KEY_A) )&&(! ((*KEYS) & KEY_B) )&&(! ((*KEYS) & KEY_START) )&&(! ((*KEYS) & KEY_SELECT) ))
      {
       EraseScreen();
       WaitForVblank();
       EraseScreen();
       WaitForVblank();
       score=0;
       level=0;
       tlines=0;
       return 0;   //if all keys are pressed it will reset the game.

      }

     count++;

     if(count%(20-level)==0)
       {

           switch(check(fx,fy+1))
           {
           case 0:change(fx,fy); end=1;
              break;
           case 1:fy=fy+1;
                change(fx,fy);
              break;
            }

       }


     print();

      }



    combine();
    clearline();
    }
   }


   return 0;
}

//******************************************************************************
void EraseScreen(void)
{
	int x,y;
	for(y = 0; y < 160; y++)
	{
		for(x = 0; x < 120; x++)

		{
			PlotPixel(x,y,0x0000);//logoData contains the color values of your pict

		}
	}
	WaitForVblank();
	Flip();
}

//******************************************************************************
void DoIntro(void)
{
	int x, y;
	for(x = 0; x < 256; x++)
		paletteMem[x] = logoPalette[x];	//loop through and store the palette from your pict
									//palette into video palette mem
	for(y = 0; y < 160; y++)
	{
		for(x = 0; x < 120; x++)

		{
			PlotPixel(x,y,logoData[y*120+x]);//logoData contains the color values of your pict

		}
	}
   Flip();
	WaitForVblank();
	FadeIn(1);
	Sleep(125);
   FadeOut(1);
}
//******************************************************************************
void DoIntro2(void)
{

	int x, y,z;


   EraseScreen();
   WaitForVblank();
   EraseScreen();
   WaitForVblank();
   FadeIn(1);

for(x = 0; x < 256; x++)
		paletteMem[x] = mikePalette[x]; //mike pallete


   for(z=1; z <=51; z++)
     {
    	for(y = 0; y < 70; y++)
	{
		for(x = 0; x < 70/2; x++)

		{
      
			PlotPixel(x+35,y+35,mikeData[(y+(z*70))*70/2+x]);//logoData contains the color values of your pict

		}
   }
    WaitForVblank();
    Flip();
    Sleep(4);
      }

   
	for(x = 0; x < 256; x++)
		paletteMem[x] = tempPalette[x];	//loop through and store the palette from your pict
									//palette into video palette mem
	for(y = 0; y < 160; y++)
	{
		for(x = 0; x < 120; x++)

		{
			PlotPixel(x,y,tempData[y*120+x]);//logoData contains the color values of your pict

		}
	}
	WaitForVblank();
	Flip();
   FadeIn(1);
	Sleep(75);

   for(y = 0; y < 160; y++)
	{
		for(x = 0; x < 120; x++)

		{
			PlotPixel(x,y,tempData[y*120+x]);//logoData contains the color values of your pict
		}
   }

   writetextslow(35,18,"-PRESS START-_");

	WaitForVblank();
	Flip();
   Sleep(20);
   WaitForStart();
   FadeOut(1);
   Sleep(10);
}

//******************************************************************************
void WaitForStart(void)
{
	while (1) // loops infinitely till they press start
		if (! ((*KEYS) & KEY_START) )
			return;
}

//******************************************************************************
void Flip(void)			// flips between the back/front buffer
{
	if(REG_DISPCNT & BACKBUFFER) //back buffer is the current buffer so we need to switch it to the font buffer
	{
		REG_DISPCNT &= ~BACKBUFFER; //flip active buffer to front buffer by clearing back buffer bit
		videoBuffer = BackBuffer; //now we point our drawing buffer to the back buffer
    }
    else //front buffer is active so switch it to backbuffer
    {
		REG_DISPCNT |= BACKBUFFER; //flip active buffer to back buffer by setting back buffer bit
		videoBuffer = FrontBuffer; //now we point our drawing buffer to the front buffer
	}
}

//******************************************************************************
void FadeOut( u32 aWait )
{
	s8 Phase;
	REG_BLDMOD = 0 | 1 | 2 | 4 | 8 | 128 | 64 | 32;
	for( Phase = 0; Phase < 16; Phase++ )
	{
		REG_COLEY = Phase;
		Sleep( aWait );
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
		Sleep ( aWait );
	}
}

//******************************************************************************
void WaitForVblank(void)	// waits for the drawer to get to end before flip
{
	while(*ScanlineCounter < 160) {	// do nothing
	}//now we are in the vblank period
}

//******************************************************************************
void background(void)
{
	int x, y;
	for(x = 0; x < 256; x++)
		paletteMem[x] = backgroundPalette[x];	//loop through and store the palette from your pict
									//palette into video palette mem
	for(y = 0; y < 160; y++)
	{
		for(x = 0; x < 120; x++)

		{
			PlotPixel(x,y,backgroundData[y*120+x]);//logoData contains the color values of your pict

		}
	}
	WaitForVblank();
  //	Flip();

}

//******************************************************************************
void print(void)
{

 int x,y;
 int a,b;

 background();

    for (y=3; y<=16; y++)
     for (x=1; x<=10; x++)
       {


       if (front[y][x] !=0)
        colors(x*5+30,y*10-20,front[y][x]);

       if (back [y][x] !=0)
        colors(x*5+30,y*10-20,back[y][x]);

       }

 //level
     if(level<10)
     {
     numbers(102,30,0);
     numbers(106,30,level);
     }

 else if(level<100)
     {
     numbers(102,30,level/10);
     numbers(106,30,level%10);
     }


//score
 if(score<10)
     {
     numbers(106,65,score);
     }
else if(score<100)
     {
     numbers(102,65,score/10);
     numbers(106,65,score%10);
     }
else if(score<1000)
     {
     numbers( 98,65,score/100);
     numbers(102,65,score/10%10);
     numbers(106,65,score%10);
     }
else if(score<10000)
     {
     numbers( 98,65,score/1000);
     numbers(102,65,score/100%10);
     numbers(106,65,score/10%10);
     numbers(110,65,score%10);
     }
else if(score<100000)
     {
     numbers( 94,65,score/10000);
     numbers( 98,65,score/1000%10);
     numbers(102,65,score/100%10);
     numbers(106,65,score/10%10);
     numbers(110,65,score%10);
     }
else if(score<1000000)
     {
     numbers( 90,65,score/100000);
     numbers( 94,65,score/10000%10);
     numbers( 98,65,score/1000%10);
     numbers(102,65,score/100%10);
     numbers(106,65,score/10%10);
     numbers(110,65,score%10);
     }


//lines
 if(tlines<10)
     {
     numbers(106,95,tlines);
     }

 else if(tlines<100)
     {
     numbers(102,95,tlines/10);
     numbers(106,95,tlines%10);
     }
 else if(tlines<1000)
     {
     numbers( 98,95,tlines/100);
     numbers(102,95,tlines/10%10);
     numbers(106,95,tlines%10);
     }


  for(a=1; a<=4; a++)
  for(b=1;b<=4; b++)
   if(shape2[a][b]!=0)
         colors(x+(5*b)-10,y+(10*a-10),shape2[a][b]);

   WaitForVblank();
   Flip();
   Sleep(1);
   //WaitForStart();

}

//******************************************************************************
void numbers(int numx, int numy,int num)
{
	int x, y;
   int temp;

   switch(num)
    {
     case 0: temp=0;
       break;
     case 1: temp=8;
       break;
     case 2: temp=16;
       break;
     case 3: temp=24;
       break;
     case 4: temp=32;
       break;
     case 5: temp=40;
       break;
     case 6: temp=48;
       break;
     case 7: temp=56;
       break;
     case 8: temp=64;
       break;
     case 9: temp=72;
       break;
     }


	for(y = 0; y < 8; y++)
		for (x = 0; x < (numbers_WIDTH/2); x++)
			PlotPixel(x+numx, y+numy, numbersData[(y+temp)*(numbers_WIDTH/2)+x]);
}

//******************************************************************************
void colors(int numx, int numy,int num)
{
	int x, y;
   int temp;

   switch(num)
    {
     case 1: temp=0;break;
     case 2: temp=10;break;
     case 3: temp=20;break;
     case 4: temp=30;break;
     case 5: temp=40;break;
     case 6: temp=50;break;
     case 7: temp=60;break;
     case 8: temp=70;break;
     }


	for(y = 0; y < 10; y++)
		for (x = 0; x < (colors_WIDTH/2); x++)
			PlotPixel(x+numx, y+numy, colorsData[(y+temp)*(colors_WIDTH/2)+x]);
}

//******************************************************************************
void shaper(int type)
{

int xrand;
xrand = REG_TM3D;
xrand *= 20077;
xrand += 12345;
xrand = ((((xrand >> 16) & 32767) * 9999) >> 15);
xrand = xrand+1;

xrand = xrand%7+1;

 switch(xrand)
   {
case 1: makeshape(0,0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,type);
      break;
case 2: makeshape(0,0,0,0,2,2,0,0,0,2,2,0,0,0,0,0,type);
      break;
case 3: makeshape(0,0,0,0,0,0,3,3,0,3,3,0,0,0,0,0,type);
      break;
case 4: makeshape(0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,type);
      break;
case 5: makeshape(0,0,0,0,0,5,5,0,0,5,5,0,0,0,0,0,type);
      break;
case 6: makeshape(0,0,0,0,0,6,0,0,0,6,6,6,0,0,0,0,type);
      break;
case 7: makeshape(0,0,0,0,0,0,7,0,7,7,7,0,0,0,0,0,type);
      break;
   }

}

//******************************************************************************
void makeshape(int a,int b,int c, int d, int e, int f, int g, int h, int i,
int j, int k,int l, int m, int n, int o, int p, int type)
 {
  if(type==1)
   {
      shape[1][1]=a;
      shape[1][2]=b;
      shape[1][3]=c;
      shape[1][4]=d;
      shape[2][1]=e;
      shape[2][2]=f;
      shape[2][3]=g;
      shape[2][4]=h;
      shape[3][1]=i;
      shape[3][2]=j;
      shape[3][3]=k;
      shape[3][4]=l;
      shape[4][1]=m;
      shape[4][2]=n;
      shape[4][3]=o;
      shape[4][4]=p;
    }
    else
     {
      shape2[1][1]=a;
      shape2[1][2]=b;
      shape2[1][3]=c;
      shape2[1][4]=d;
      shape2[2][1]=e;
      shape2[2][2]=f;
      shape2[2][3]=g;
      shape2[2][4]=h;
      shape2[3][1]=i;
      shape2[3][2]=j;
      shape2[3][3]=k;
      shape2[3][4]=l;
      shape2[4][1]=m;
      shape2[4][2]=n;
      shape2[4][3]=o;
      shape2[4][4]=p;
      }
 }

//******************************************************************************
  void change(int fx, int fy)
    {
    clearfront();

     front[fy][fx]    =shape[1][1];
     front[fy][fx+1]  =shape[1][2];
     front[fy][fx+2]  =shape[1][3];
     front[fy][fx+3]  =shape[1][4];
     front[fy+1][fx]  =shape[2][1];
     front[fy+1][fx+1]=shape[2][2];
     front[fy+1][fx+2]=shape[2][3];
     front[fy+1][fx+3]=shape[2][4];
     front[fy+2][fx]  =shape[3][1];
     front[fy+2][fx+1]=shape[3][2];
     front[fy+2][fx+2]=shape[3][3];
     front[fy+2][fx+3]=shape[3][4];
     front[fy+3][fx]  =shape[4][1];
     front[fy+3][fx+1]=shape[4][2];
     front[fy+3][fx+2]=shape[4][3];
     front[fy+3][fx+3]=shape[4][4];
    }
//******************************************************************************
 void move(void)
    {
    int a,b;
     for (a=1;a<=4;++a)
      for (b=1;b<=4;++b)
        temp[a][b]=shape[a][b];

      shape[1][1]=temp[1][4];
      shape[1][2]=temp[2][4];
      shape[1][3]=temp[3][4];
      shape[1][4]=temp[4][4];
      shape[2][1]=temp[1][3];
      shape[2][2]=temp[2][3];
      shape[2][3]=temp[3][3];
      shape[2][4]=temp[4][3];
      shape[3][1]=temp[1][2];
      shape[3][2]=temp[2][2];
      shape[3][3]=temp[3][2];
      shape[3][4]=temp[4][2];
      shape[4][1]=temp[1][1];
      shape[4][2]=temp[2][1];
      shape[4][3]=temp[3][1];
      shape[4][4]=temp[4][1];
    }

//******************************************************************************
void clearfront(void)
 {
 int x,y;
    for (y=0; y<=16; y++)
    for (x=0; x<=10; x++)
      {
      front[y][x]=0;
      }
 }

//******************************************************************************
int check(int g, int h)
 {
 int temp=1;
 int a,b;

 change(g,h);


  for (a=0;a<=3;a++)
    for (b=0;b<=3;b++)
      if ((front[h+a][g+b]!=0)&&(back[h+a][g+b]!=0))
         {
         temp=0;
         }
         return temp;
 }
//******************************************************************************
void combine(void)
  {
  int a,b;
  for (a=1;a<=16;a++)
    for (b=1;b<=10;b++)
      if (front[a][b]!=0)
        back[a][b]=front[a][b];
  }

//******************************************************************************
//
void clearline(void)
{
 int line=0;
 int lines=0;
 int tempscore=0;
 int a,b;

   for (a=1;a<=16;++a)
    {
     line =0;
    for (b=1;b<=10;++b)
     {
     if (back[a][b]!=0)
       line++;
     if (line==10)
       {lines++;
       movedown(a);
       tlines++;
       }
     }
     }

    tempscore= lines*lines*10;
    score=score+tempscore;

    level=tlines/20 +1;

}

//******************************************************************************
//
void movedown(int j)
{
int a,b;

    for (a=j;a>=1;a=a-1)
     for (b=1;b<=10;b++)
        back[a][b]=back[a-1][b];
    for (b=1;b<=10;b++)
        back[1][b]=0;
}
//******************************************************************************
void gameover(void)
{
	int x, y;
   int temp=score;

   Sleep(10);
	for(x = 0; x < 256; x++)
		paletteMem[x] = gameoverPalette[x];	//loop through and store the palette from your pict
									//palette into video palette mem
	for(y = 0; y < 160; y++)
	{
		for(x = 0; x < 120; x++)

		{
			PlotPixel(x,y,gameoverData[y*120+x]);//logoData contains the color values of your pict

		}
	}
	WaitForVblank();
	Flip();
   FadeIn(1);
	Sleep(125);
   FadeOut(1);

}
//******************************************************************************
void save(void)
 {
 int x,y;
 int b,c;
 int a;
 int temp;
 int place=0;

char namesent[10];
char names[10][10];
int numbers[5][10];
int numbersint[5];

for(y=0;y<5;y++)
  numbersint[y]=0;

//creates a temporary list of the memory
 for (y=0; y<10;y++)
  for(x=0; x<10;x++)
   names[y][x]=pSaveMemory[y*10+x];

 for(y=5; y<10; y++)
   for(x=0; x<10; x++)
    {
     switch(pSaveMemory[y*10+x])
      {
      case '0' :numbers[y-5][x]=0; break;
      case '1' :numbers[y-5][x]=1; break;
      case '2' :numbers[y-5][x]=2; break;
      case '3' :numbers[y-5][x]=3; break;
      case '4' :numbers[y-5][x]=4; break;
      case '5' :numbers[y-5][x]=5; break;
      case '6' :numbers[y-5][x]=6; break;
      case '7' :numbers[y-5][x]=7; break;
      case '8' :numbers[y-5][x]=8; break;
      case '9' :numbers[y-5][x]=9; break;
      }

    }

//creates a list of numbers to sort
 for(y=0;y<5;y++)
  {
   b=0;

  while(names[y+5][b]!='_')
   b++;
   c=b;

   for(x=0; x<b; x++)
     {
     c--;
     numbersint[y]=numbersint[y] + (numbers[y][x] * pow(10,c));
     }
     numbersint[y]=numbersint[y] + numbers[y][b];
  }

//sort

c=5;
for(x=4; x>=0; x--)
  if(score>numbersint[x])
    c=x;

//    writenumbersslow(0,0,c);
//    WaitForStart();

if(c!=5)
{
for(y=4; y>=c; y--)
  for(x=0; x<10; x++)
   {
     if(y!=0)
     {
   names[y+5][x]=names[y+4][x];
   names[y][x]=names[y-1][x];
     }
   }
   
  nameinput(namesent);

 for(a=0;a<10; a++)
  {
  names[c][a]=' ';
  names[c+5][a]=' ';
  }

  b=0;
 while(namesent[b]!='_')
  b++;

 for(a=0; a<=b; a++)
  names[c][a]=namesent[a];

 b=0;
 temp=score;

 while (temp!=0)
 {
  temp=temp/10;
  b++;
 }
  b--;

for(a=b; a>=0; a--)
{
  if(a==b)
    {
      switch(score/pow(10,a))
        {
        case 1: names[c+5][place]='1';break;
        case 2: names[c+5][place]='2';break;
        case 3: names[c+5][place]='3';break;
        case 4: names[c+5][place]='4';break;
        case 5: names[c+5][place]='5';break;
        case 6: names[c+5][place]='6';break;
        case 7: names[c+5][place]='7';break;
        case 8: names[c+5][place]='8';break;
        case 9: names[c+5][place]='9';break;
        case 0: names[c+5][place]='0';break;
        }
       place++;
    }

  else if(a==0)
   {
     switch(score%10)
        {
        case 1: names[c+5][place]='1';break;
        case 2: names[c+5][place]='2';break;
        case 3: names[c+5][place]='3';break;
        case 4: names[c+5][place]='4';break;
        case 5: names[c+5][place]='5';break;
        case 6: names[c+5][place]='6';break;
        case 7: names[c+5][place]='7';break;
        case 8: names[c+5][place]='8';break;
        case 9: names[c+5][place]='9';break;
        case 0: names[c+5][place]='0';break;
        }
    place++;
   }

  else
    {
     switch(score/pow(10,a)%10)
        {
        case 1: names[c+5][place]='1';break;
        case 2: names[c+5][place]='2';break;
        case 3: names[c+5][place]='3';break;
        case 4: names[c+5][place]='4';break;
        case 5: names[c+5][place]='5';break;
        case 6: names[c+5][place]='6';break;
        case 7: names[c+5][place]='7';break;
        case 8: names[c+5][place]='8';break;
        case 9: names[c+5][place]='9';break;
        case 0: names[c+5][place]='0';break;
        }
     place++;
    }
}
 names[c+5][b+1]='_';

}




//prints out the highscores
EraseScreen();
WaitForVblank();
EraseScreen();
WaitForVblank();

writetext(34,20,"-HIGH SCORES-_");

for(y=0;y<5;y++)
{
 b=0;

 while(names[y][b]!='_')
  b++;

  for(x=0;x<b;x++)
  {
      letters(30+(4*x),40+(15*y),names[y][x]);
  }
}

for(y=5;y<10;y++)
{
 b=0;

 while(names[y][b]!='_')
  b++;

  for(x=0;x<b;x++)
  {
      letters(75+(4*x),40+(15*(y-5)),names[y][x]);
  }
}

 WaitForVblank();
 Flip();


//clears the memory
 for(x=0; x<=100; x++)
  {
  pSaveMemory[x]=' ';
  }

//saves the memory again
for (y=0; y<10;y++)
  for(x=0; x<10;x++)
   pSaveMemory[y*10+x]=names[y][x];

  FadeIn(1);


  Sleep(120);
  FadeOut(1);

  EraseScreen();
  WaitForVblank();
  EraseScreen();
  WaitForVblank();

 }
//******************************************************************************
void letters(int numx, int numy, char num)
{
	int x, y;
   int temp;

   switch(num)
    {
     case ' ' : temp=0; break;
     case 'A' : temp=8; break;
     case 'B' : temp=16; break;
     case 'C' : temp=24; break;
     case 'D' : temp=32; break;
     case 'E' : temp=40; break;
     case 'F' : temp=48; break;
     case 'G' : temp=56; break;
     case 'H' : temp=64; break;
     case 'I' : temp=72; break;
     case 'J' : temp=80; break;
     case 'K' : temp=88; break;
     case 'L' : temp=96; break;
     case 'M' : temp=104; break;
     case 'N' : temp=112; break;
     case 'O' : temp=120; break;
     case 'P' : temp=128; break;
     case 'Q' : temp=136; break;
     case 'R' : temp=144; break;
     case 'S' : temp=152; break;
     case 'T' : temp=160; break;
     case 'U' : temp=168; break;
     case 'V' : temp=176; break;
     case 'W' : temp=184; break;
     case 'X' : temp=192; break;
     case 'Y' : temp=200; break;
     case 'Z' : temp=208; break;
     case 'a' : temp=216; break;
     case 'b' : temp=224; break;
     case 'c' : temp=232; break;
     case 'd' : temp=240; break;
     case 'e' : temp=248; break;
     case 'f' : temp=256; break;
     case 'g' : temp=264; break;
     case 'h' : temp=272; break;
     case 'i' : temp=280; break;
     case 'j' : temp=288; break;
     case 'k' : temp=296; break;
     case 'l' : temp=304; break;
     case 'm' : temp=312; break;
     case 'n' : temp=320; break;
     case 'o' : temp=328; break;
     case 'p' : temp=336; break;
     case 'q' : temp=344; break;
     case 'r' : temp=352; break;
     case 's' : temp=360; break;
     case 't' : temp=368; break;
     case 'u' : temp=376; break;
     case 'v' : temp=384; break;
     case 'w' : temp=392; break;
     case 'x' : temp=400; break;
     case 'y' : temp=408; break;
     case 'z' : temp=416; break;
     case '0' : temp=424; break;
     case '1' : temp=432; break;
     case '2' : temp=440; break;
     case '3' : temp=448; break;
     case '4' : temp=456; break;
     case '5' : temp=464; break;
     case '6' : temp=472; break;
     case '7' : temp=480; break;
     case '8' : temp=488; break;
     case '9' : temp=496; break;
  	  case '!' : temp=504; break;
	//case '"': temp=512; break;
	  case '#' : temp=520; break;
	  case '$' : temp=528; break;
	  case '%' : temp=536; break;
	  case '&' : temp=544; break;
  	//case ''' : temp=552; break;
	  case '(' : temp=560; break;
	  case ')' : temp=568; break;
	  case '*' : temp=576; break;
	  case '+' : temp=584; break;
	  case ',' : temp=592; break;
	  case '-' : temp=600; break;
	  case '.' : temp=608; break;
	  case '/' : temp=616; break;
	  case ':' : temp=624; break;
	  case ';' : temp=632; break;
	  case '<' : temp=640; break;
	  case '=' : temp=648; break;
	  case '>' : temp=656; break;
	  case '?' : temp=664; break;
     default: temp=0; break;
     }


	for(y = 0; y < 8; y++)
		for (x = 0; x < (letters_WIDTH/2); x++)
			PlotPixel(x+numx, y+numy, lettersData[(y+temp)*(letters_WIDTH/2)+x]);

}

//******************************************************************************
void writetext(int x, int y, char *message)
{

int a;
int b=0;

while(message[b]!='_')
  b++;

 for (a=0; a<b; a++)
 {
 letters(x+(a*4),y, message[a]);
 }
}

//******************************************************************************
void writetextslow(int x, int y, char *message)
{

int a;
int b=0;

while(message[b]!='_')
  b++;

 for (a=0; a<b; a++)
 {
 letters(x+(a*4),y, message[a]);
 Sleep(2);   //sleep and flip only for typeing like effect
 WaitForVblank();
 Flip();

 letters(x+(a*4),y, message[a]);
 Sleep(2);
 WaitForVblank();
 Flip();

 }
}

//******************************************************************************
void writetextblock(char *message, int sleepamount)
{
int list[16][30];
int x,y,z;

EraseScreen();
WaitForVblank();
EraseScreen();
WaitForVblank();

  z=0;
  for(y=0; y<16; y++)
   for(x=0; x<30; x++)
    {
      if(message[z]=='_')
        { y=16; x=30;}
      else
      {
    list[y][x]=message[z];
    z++;
    letters((4*x),(10*y), list[y][x]);
      }
    }

  WaitForVblank();  
  Flip();
  FadeIn(1);
  Sleep(sleepamount);
  FadeOut(1);

}

//******************************************************************************
void writetextmemory(int x, int y, char *message)
{

int a;
int b=0;

while(message[b]!='_')
  b++;

 for (a=0; a<b; a++)
 {
 pSaveMemory[(y*10)+x+a]= message[a];
 }
 pSaveMemory[(y*10)+x+b]='_';
}

//******************************************************************************
void writenumbers(int x, int y, int num)
{
int a;
int b=0;
int temp=num;
int place=0;

if (num<10)
  {
    numbers1(x,y,num);
  }
else
  {

while (temp!=0)
 {
  temp=temp/10;
  b++;
 }
 b--;

for(a=b; a>=0; a--)
{
  if(a==b)
    {numbers1(x,y,num/pow(10,a));
     place++;}

  else if(a==0)
   {numbers1(x+(place*4),y,num%10);
    place++;}

  else
    {numbers1(x+(place*4),y,(num/pow(10,a))%10);
     place++;}
}
   }

}

//******************************************************************************
void writenumbersslow(int x, int y, int num)
{
int a;
int b=0;
int temp=num;
int place=0;

if (num<10)
  {
    numbers1(x,y,num);
    Sleep(2);
    WaitForVblank();
    Flip();
    numbers1(x,y,num);
    Sleep(2);
    WaitForVblank();
    Flip();
  }
else
  {

while (temp!=0)
 {
  temp=temp/10;
  b++;
 }
 b--;

for(a=b; a>=0; a--)
{
  if(a==b)
    {numbers1(x,y,num/pow(10,a));
     Sleep(2);
     WaitForVblank();
     Flip();
     numbers1(x,y,num/pow(10,a));
     Sleep(2);
     WaitForVblank();
     Flip();
     place++;}

  else if(a==0)
   {numbers1(x+(place*4),y,num%10);
    Sleep(2);
    WaitForVblank();
    Flip();
    numbers1(x+(place*4),y,num%10);
    Sleep(2);
    WaitForVblank();
    Flip();
    place++;}

  else
    {numbers1(x+(place*4),y,(num/pow(10,a))%10);
     Sleep(2);
     WaitForVblank();
     Flip();
     numbers1(x+(place*4),y,(num/pow(10,a))%10);
     Sleep(2);
     WaitForVblank();
     Flip();

     place++;}
}
   }

}

//******************************************************************************
void numbers1(int numx, int numy,int num)
{
	int x, y;
   int temp;

   switch(num)
    {
     case 0 : temp=424; break;
     case 1 : temp=432; break;
     case 2 : temp=440; break;
     case 3 : temp=448; break;
     case 4 : temp=456; break;
     case 5 : temp=464; break;
     case 6 : temp=472; break;
     case 7 : temp=480; break;
     case 8 : temp=488; break;
     case 9 : temp=496; break;
    }

    for(y = 0; y < 8; y++)
		for (x = 0; x < (letters_WIDTH/2); x++)
			PlotPixel(x+numx, y+numy, lettersData[(y+temp)*(letters_WIDTH/2)+x]);
}

//******************************************************************************
int pow(int num, int exp)
{
 int z;
 int temp=num;

 if (exp==0)
  return 0;


 for (z=1; z<exp; z++)
   num=num*temp;

  return num;
}



//******************************************************************************
void memory(void)
{
int x,y;

 EraseScreen();
 WaitForVblank();
 EraseScreen();
 WaitForVblank();
 FadeIn(1);
 for(x = 0; x < 256; x++)
		paletteMem[x] = gameoverPalette[x];


//check for previous save if not create new one
    if ((pSaveMemory[200]!='s')&&(pSaveMemory[201]!='a')&&(pSaveMemory[202]!='v')&&(pSaveMemory[203]!='e'))
   {
    writetextmemory(200,0,"save_");

    writetextmemory(0,0,"MadMan_");
    writetextmemory(0,1,"Werner_");

    writetextmemory(0,2,"Satan_");
    writetextmemory(0,3,"Zombie_");

    writetextmemory(0,4,"John_");
    writetextmemory(0,5,"5000_");

    writetextmemory(0,6,"2500_");
    writetextmemory(0,7,"1500_");

    writetextmemory(0,8,"1000_");
    writetextmemory(0,9,"100_");

    EraseScreen();
    WaitForVblank();
    EraseScreen();
    WaitForVblank();

    writetextslow(10,10,"New Memory Created_");
   }
   else if ((pSaveMemory[200]=='s')&&(pSaveMemory[201]=='a')&&(pSaveMemory[202]=='v')&&(pSaveMemory[203]=='e'))
    {
    EraseScreen();
    WaitForVblank();
    EraseScreen();
    WaitForVblank();

    writetextslow(10,10,"Loaded Memory_");
    }
//end of checking

 Sleep(40);
 FadeOut(1);

 EraseScreen();
 WaitForVblank();
 EraseScreen();
 WaitForVblank();

}

//******************************************************************************
void nameinput(char namesent[])
{

 int x,y;
 int z=0;
 int a;
 int end=0;
 char list[8][10];
 int posx=0,posy=0,posz=0;
 char name[10];

 char *characters="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$%&()*+,-./:<>? ";

 for(a=0; a<10; a++)
  name[10]=' ';

 EraseScreen();
 WaitForVblank();
 FadeIn(1);
 EraseScreen();
 WaitForVblank();

while(end!=1)
 {

for(a=0; a<2; a++)
 {
  z=0;
  for(y=0; y<8; y++)
   for(x=0; x<10; x++)
    {
    list[y][x]=characters[z];
    z++;
    letters( 22+(8*x), 10+(15*y), list[y][x]);
    }

    writetext(20,150,"Press Start to Finish_");
   
  if(a==1)
     for(y = 0; y < 8; y++)
		for (x = 0; x < (letters_WIDTH/2); x++)
			PlotPixel(x+22+(8*posx), y+10+(15*posy), lettersData[(y)*(letters_WIDTH/2)+x]);

writetext(20,135,"          _");

for(x=0;x<posz;x++)
  letters(20+(4*x),135,name[x]);

  if (! ((*KEYS) & KEY_RIGHT) )
      if (posx!=9)
       {posx++;Sleep(5);}

  if (! ((*KEYS) & KEY_LEFT) )
      if (posx!=0)
       {posx--;Sleep(5);}

  if (! ((*KEYS) & KEY_DOWN) )
      if (posy!=7)
       {posy++;Sleep(5);}

  if (! ((*KEYS) & KEY_UP) )
      if (posy!=0)
       {posy--;Sleep(5);}

  if (! ((*KEYS) & KEY_A) )
      if (posz!=8)
       {
       name[posz]=list[posy][posx];
       posz++;
       Sleep(5);
       }

  if (! ((*KEYS) & KEY_B) )
      if (posz!=0)
       {
       name[posz]=' ';
       posz--;
       Sleep(5);
       }

  if (! ((*KEYS) & KEY_START) )
       end=1;

  WaitForVblank();     
  Flip();
  Sleep(5);
  }

 
  }

  for(x=0; x<posz; x++)
   namesent[x]=name[x];
   namesent[posz]='_';

}

//******************************************************************************
void choices(void)
{
 int x;
 int end=0;

 EraseScreen();
 WaitForVblank();
 EraseScreen();
 WaitForVblank();
 FadeIn(0);

 writetext(10,50,"-Start   Play Game_");
 writetext(10,70,"-Select  High Scores_");
 writetext(10,90,"-L+R     Clear The Memory_");
 WaitForVblank();
 Flip();
 FadeIn(1);

 while (end!=1)
 {

   if ((! ((*KEYS) & KEY_L) )&&(! ((*KEYS) & KEY_R) ))
      {
       for(x=0; x<=210; x++)
         pSaveMemory[x]=' ';
       memory();
       end=1;

      }

   if (! ((*KEYS) & KEY_SELECT) )
     {
      save();
      end=1;
     }

   if (! ((*KEYS) & KEY_START) )
     {end=1;}

 }

  FadeOut(1);

}

void Sleep(int i) {
    int count = i * 60 * 60;

    while(count--) {
        WaitForVblank();
    }
}
