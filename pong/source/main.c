#include "gba.h"
#include "screenmode.h"
#include "keypad.h"
#include "gfx.h"
#include "timers.h"
#include "gfx/letters.h"
#include "gfx/shapes.h"


//to save game:
#define GAMEPAK_RAM  ((u8*)0x0E000000)
u8 *pSaveMemory = GAMEPAK_RAM;
//example to save: pSaveMemory[0] = 1;


// some memory location defines
u16* FrontBuffer = (u16*)0x6000000;
u16* BackBuffer = (u16*)0x600A000;
u16* videoBuffer;
volatile u16* ScanlineCounter = (volatile u16*)0x4000006;

u16* paletteMem = (u16*)0x5000000;	//PalMemory is 256 16 bit BGR values starting at 0x5000000

//struct to simulate a class
struct genericObject
{
  int x, y;
  int xSpeed,ySpeed;
};
struct genericObject *pMyObject, myObject;
struct genericObject leftPaddle, rightPaddle;

void move(struct genericObject *a)
{
 a->x=a->x + a->xSpeed;
 a->y=a->y + a->ySpeed;
}

int count;
int score1,score2;
int end;



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
void WaitForStart(void);	// waits for the start key to be pressed
void Flip(void);			// flips between the back front buffer
void WaitForVblank(void);	// waits for the drawer to get to end before flip
void FadeOut( u32 aWait ); //fades out
void FadeIn( u32 aWait ); // fades in
void numbers(int numx, int numy,int num); //prints out the numbers with black background
void letters(int numx, int numy, char num); //used to print out the letters
void writetext(int x, int y, char *message); //used to print out the letters
void writetextblock(char *message); //prints out letters onto the whole screen;
void writenumbers(int x, int y, int num); // used to print out numbers
int pow2(int num, int exp);  //raises a number to a certain power... made to work with my program
int random(int min, int max);
void shapes(int numx, int numy,int w, int h,int num);
void keyCheck();

void drawSquare(int xpos, int ypos, int l, int w, int color);

void paint();
void checkCollision();
void intro();
void logo();

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
int main (void)
{
  int x;
  pMyObject=&myObject;

  //start timer
  REG_TM3CNT = TIME_FREQUENCY_1024 | TIME_ENABLE;
  //zero the timer
  REG_TM3D = 0;

  SetMode(MODE_4 | BG2_ENABLE );   //set to mode 4

  for(x = 0; x < 256; x++)
		paletteMem[x] = lettersPalette[x];   //shapesPalette[x];
      
    while(1) {
//some code
   end=1;

   leftPaddle.x=8;
   leftPaddle.y=56;

   rightPaddle.x=224;
   rightPaddle.y=56;

   myObject.x=116;
   myObject.y=72;

//some code

   EraseScreen();
   WaitForVblank();
   EraseScreen();
   WaitForVblank();
   
   logo();
   intro();

   paint();
   Flip();
   FadeIn(3);
   myObject.xSpeed=2;

 while(end)
   {

   keyCheck();
   checkCollision();
   move(pMyObject);
   paint();
   WaitForVblank();
   Flip();
   //Sleep(1);

   }
   }
  return 0;
}

//******************************************************************************
void intro()
{
  int x=0; 
  int loop=1;
  int count=0;

 while(loop)
  {
  if (! ((*KEYS) & KEY_START) )
     {
      loop=0;
      //Sleep(20);
     }
  else
  {

  drawSquare(0,0,240,160,0x0000);

  drawSquare(5,20,20,80,0xf0f0);
  drawSquare(35,20,20,40,0xf0f0);
  drawSquare(15,20,20,5,0xf0f0);
  drawSquare(15,55,20,5,0xf0f0);


  drawSquare(65,20,20,80,0xf0f0);
  drawSquare(95,20,20,80,0xf0f0);
  drawSquare(75,20,20,5,0xf0f0);
  drawSquare(75,95,20,5,0xf0f0);

  drawSquare(125,20,20,80,0xf0f0);
  drawSquare(165,20,10,80,0xf0f0);
  drawSquare(150,50,10,10,0xf0f0);
  drawSquare(155,55,10,10,0xf0f0);
  drawSquare(140,40,10,15,0xf0f0);
  drawSquare(150,45,5,5,0xf0f0);
  drawSquare(160,65,5,5,0xf0f0);


  drawSquare(185,20,20,80,0xf0f0);
  drawSquare(215,20,20,80,0xf0f0);
  drawSquare(195,20,20,5,0xf0f0);
  drawSquare(195,95,20,5,0xf0f0);
  drawSquare(215,55,20,5,0x0000);
  drawSquare(215,70,5,20,0x0000);


  writetext(80,140,"By Werner Mendizabal_");

  writetext(x,10+(x%3),"PONG_");
  writetext(-x,10+(x%3),"PONG_");
  writetext(x,110+(x%3),"PONG_");
  writetext(x+4,50+(x%3),"PONG_");
  writetext(-x+30,100+(x%3),"PONG_");
  writetext(-x,10+(x%3),"PONG_");
  writetext(x+80,70+(x%3),"PONG_");
  writetext(-x+99,10+(x%3),"PONG_");

  writetext(72,128+(x%3),"Press Start_");
  x+=2;
  Flip();
  Sleep(1);

  if(count==1)
   {
   FadeIn(3);
   count++;
   }
  else if(count==0)
   count++;

  }
  }
  FadeOut(3);
}

//******************************************************************************
void logo()
{

unsigned short int x,y,z;

int array[13][9]=
{
{0,0,1,0,0,0,1,0,0},
{0,1,0,0,0,0,0,1,0},
{1,0,0,1,1,1,0,0,1},
{1,0,1,1,1,1,1,0,1},
{1,1,0,1,1,1,1,1,1},
{0,0,1,1,1,1,1,0,0},
{0,1,0,0,1,0,0,1,0},
{0,0,1,1,1,1,1,0,0},
{0,0,0,1,1,1,0,0,0},
{0,0,1,0,1,0,1,0,0},
{0,0,0,1,0,1,0,0,0},
{0,0,0,1,1,1,0,0,0},
{0,0,0,0,1,0,0,0,0}
};

for(z=0;z<2;z++)
{
drawSquare(0,0,240,160,0x0000);
WaitForVblank();
writetext(48,132,"MADMAN PRODUCTIONS_");
writetext(88,144,"PRESENTS_");

for(y=0; y<13; y++)
 for (x=0; x<9; x++)
   if (array[y][x]==1)
    drawSquare((x*8)+80,y*8+16,8,8,0xffff);

  WaitForVblank();
  Flip();
}
  FadeIn(3);
  Sleep(140);
  FadeOut(3);


}


//******************************************************************************
void EraseScreen(void)
{
	int x,y;
	for(y = 0; y < 160; y++)
	{
		for(x = 0; x < 120; x++)

		{
			PlotPixel(x,y,0xf0f0); //f0f0

		}
	}
	WaitForVblank();
  
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
			PlotPixelTransparent(x+numx, y+numy, lettersData[(y+temp)*(letters_WIDTH/2)+x]);     //Transparent

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
 letters(x/2+(a*4),y, message[a]);
 }
}

//******************************************************************************
void writetextblock(char *message)
{
int list[16][30];
int x,y,z;

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

}

//******************************************************************************
void writenumbers(int x, int y, int num)
{
int a;
int b=0;
int temp=num;
int place=0;
x=x/2;

if (num<10)
  {
    numbers(x,y,num);
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
    {numbers(x,y,num/pow2(10,a));
     place++;}

  else if(a==0)
   {numbers(x+(place*4),y,num%10);
    place++;}

  else
    {numbers(x+(place*4),y,(num/pow2(10,a))%10);
     place++;}
}
   }

}


//******************************************************************************
void numbers(int numx, int numy,int num)
{
	int x, y;
   int temp=0;

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
			PlotPixelTransparent(x+numx, y+numy, lettersData[(y+temp)*(letters_WIDTH/2)+x]);
}

//******************************************************************************
int pow2(int num, int exp)
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
int random(int min, int max)
{

int xrand;

xrand = REG_TM3D;
xrand *= 20077;
xrand += 12345;
xrand = ((((xrand >> 16) & 32767) * 9999) >> 15);
xrand = xrand+1;

return xrand %(max-min+1)+min;

}

//******************************************************************************
void shapes(int numx, int numy,int w, int h,int num)
{

   //8 is the height

   int rCutOff = 0;

	int x, y;
   int temp;

   int tempx=0, tempy=0;

   temp=num*h;

   if (numx<0)
    tempx=-numx/2;

   if (numy<0)
    tempy=-numy;

   if(numx+w>240)
    rCutOff=240-(numx+w);

   if(numx>240)
    tempx=w;

   if (numy>160)
    tempy=h;


	for(y=tempy; y < h; y++)
		for (x=tempx; x < (w+rCutOff)/2; x++)
			PlotPixelTransparent(x+numx/2, y+numy, shapesData[(y+temp)*(w/2)+x]);
}

//******************************************************************************
void keyCheck()
{

    if (! ((*KEYS) & KEY_START) )
     {     }

    if (! ((*KEYS) & KEY_UP) )
     { leftPaddle.y-=4;     }

    if (! ((*KEYS) & KEY_DOWN) )
     { leftPaddle.y+=4;    }

    if (! ((*KEYS) & KEY_LEFT) )
     {     }

     if (! ((*KEYS) & KEY_RIGHT) )
     {     }

    if (! ((*KEYS) & KEY_A) )
     {  rightPaddle.y-=4;   }

    if (! ((*KEYS) & KEY_B) )
     {  rightPaddle.y+=4;   }

    if (! ((*KEYS) & KEY_L) )
     {     }

    if (! ((*KEYS) & KEY_R) )
     {     }

    if(leftPaddle.y<0)
     {
      leftPaddle.y=0;
     }

    if(leftPaddle.y>160-32)
     {
      leftPaddle.y=160-32;
     }

    if(rightPaddle.y<0)
     {
      rightPaddle.y=0;
     }

    if(rightPaddle.y>160-32)
     {
      rightPaddle.y=160-32;
     }


}
//******************************************************************************
void drawSquare(int xpos, int ypos, int l, int w, int color)
{
	int x,y;
	for(y = ypos; y < ypos+w; y++)
	{
		for(x = xpos/2; x < (xpos+l)/2; x++)

		{
			PlotPixel(x,y,color);//logoData contains the color values of your pict

		}
	}
}


//******************************************************************************
void paint()
{
  EraseScreen();
  
  writenumbers(100,8,score1);
  writenumbers(132,8,score2);


  drawSquare(116,0,8,160,0xffff);
  drawSquare(leftPaddle.x,leftPaddle.y,8,32,0xffff);
  drawSquare(rightPaddle.x,rightPaddle.y,8,32,0xffff);
  shapes(myObject.x,myObject.y,8,8,0);

}

//******************************************************************************
void checkCollision()
{

  if((myObject.y>rightPaddle.y)&&(myObject.y<rightPaddle.y+32)&&(myObject.x+8>=rightPaddle.x)) //    &&(myObject.x<rightPaddle.x)
     {
     myObject.xSpeed=-myObject.xSpeed;
     myObject.ySpeed=random(-4,4);
     myObject.x=216;
     count++;
     }

  if((myObject.y>leftPaddle.y)&&(myObject.y<leftPaddle.y+32)&&(myObject.x-8<=leftPaddle.x))                   //&&(myObject.x>leftPaddle.x)
     {
     myObject.xSpeed=-myObject.xSpeed;
     myObject.ySpeed=random(-4,4);
     myObject.x=16;
     count++;
     }

  if(myObject.y<0)
    myObject.ySpeed=-myObject.ySpeed;

  if(myObject.y+8>160)
    myObject.ySpeed=-myObject.ySpeed;

  if(myObject.x>240)
    {
     score1++;
     //myObject.xSpeed=2;
     myObject.ySpeed=1;

     myObject.x=0;
     myObject.y=0;

     EraseScreen();
     writetext(40,64,"Player One Scored!!!_");
     Flip();
     Sleep(100);

     if(score1==5)
     {
     EraseScreen();
     writetext(40,64,"Player One Wins!!!_");
     Flip();
     Sleep(100);
     end=0;
     }


    }

  if(myObject.x<0)
    {
     score2++;
     //myObject.xSpeed=-2;
     myObject.ySpeed=1;

     myObject.x=240;
     myObject.y=0;

     EraseScreen();
     writetext(40,64,"Player Two Scored!!!_");
     Flip();
     Sleep(100);

     if(score2==5)
     {
     EraseScreen();
     writetext(40,64,"Player Two Wins!!!_");
     Flip();
     Sleep(100);
     end=0;
     }

    }

  if(count>5)
    {
    if(myObject.xSpeed>0)
     myObject.xSpeed=myObject.xSpeed+1;
    else if(myObject.xSpeed<0)
     myObject.xSpeed=myObject.xSpeed-1;

     count=0;
    }
}

void Sleep(int i) {
    int count = i * 60 * 60;

    while(count--) {
        WaitForVblank();
    }
}






























