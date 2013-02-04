 /********************************************************
*	
* no title
*
********************************************************/

///////////////////Includes//////////////////////
#include "lib.h"	
#include "myfunctions.h"
#include "boyscout.cpp"
//graphic files
#include "gfx/logo.h"
#include "gfx/megatroid.h"
#include "gfx/gbax.h"
#include "gfx/terminal.h" //letters italic serif
#include "gfx/shapes.h"
#include "gfx/map1.h"
#include "gfx/map2.h"
#include "gfx/guy.h" 
#include "gfx/background.h" 
#include "gfx/backMap.h" 
#include "gfx/textmap.h" 
#include "gfx/bloob.h" 
#include "gfx/snake.h" 
#include "gfx/foreground.h" 
 
//sound files
#include "sound/tune.cpp"
#include "sound/boss.cpp"
#include "sound/rock.cpp"
#include "sound/music.cpp"
#include "sound/fire.h"
#include "sound/jump.h"
#include "sound/mega.h"
#include "sound/swap.h"


//multiboot support
#define MULTIBOOT volatile const u8 __gba_multiboot;
MULTIBOOT

#define SpriteCollision(x1,y1,x2,y2,x3,y3,x4,y4) ((x3)<(x2))&&((x4)>(x1))&&((y3)<(y2))&&((y4)>(y1)) 
#define SPRIORITY 0x2<<10
#define FREQ 190
#define UP 1
#define DOWN 0

//////////////////Globals/////////////////////////

struct maps
{
s16 xoffset;  //screen x and y pos
s16 yoffset;
u8 x;        //screen map array x and y;
u8 y;
u16 w;   //size of the current map
u16 h; 
}Map;

unsigned char currentMap[100][100];

struct player
{
s16	x;
s16	y;
u8 jump;        //jump counter
u8 canjump;     //can sprite jump
u8 jumpAmount;  //how much to jump
u8 lor;         //sprite facing left or right

u8 shoot; //counter for the shooting controls graphics
}guy;

struct badguy
{
s16 x;
s16 y;
u8 w;
u8 h;
u8 lor;
}baddy[5];


struct bullets
{
s8 velocity;
s16 x;
s16 y;
}bullet[3];

u8 currentBullet;

u8 end=1; //used to end game or restart the game
u8 count; //control the graphics, changes every time it reaches 10 or something like that

s8 millisec, sec, min;
u8 kills;

u8 bL; //button L

u8 difficulty;
u8 dimension=1;

const unsigned char monsterstack[2][20]={
{34,72,38, 3,62,88,26,10,51,52,34,90,58,90,25,20,48,47,73, 5},
{23,24, 6,62,96,97,74,71,86,20,23, 9,37,37,73,83,74,81, 8,28}
};

u8 currentstack1;
u8 currentstack2=10;

///////////////Function Prototypes////////////////
void Setup(void);
void SetupMusic(void);
void mapUpdate(void);
void KeyCheck(void);
void ShootBullet();
u8 checkMapCollision(u16 x, u16 y, u8 w, u8 h);
void mapBoundery(); //make sure map i
u8 insideScreen(struct badguy b);
u8 guyCollision(u8 b);
u8 bulletCollision(u8 b, u8 bullet);
void DoIntro(void);
void scroll(u8 d); //used to move map with text on it
void textWindow(char *message);
void textWindow(char *message);
void movebaddys();
void movebullets();
void updateTime(u8 d);
void loadMap(u8 m);
void options(void);
///////////////// Main ///////////////////////////
int main(void)
{
	u8 x,y;

	
	//display icon
  SetupMusic();
  DoIntro();
  BoyScoutStopSong();

	

  
	 //Setup the graphics
	 Setup();
   //setup ends here
   
//load the map layers
 for(y=0; y<22; y++)
	for(x=0; x<32; x++)
	 {
	 DrawTile(x,y,frontMap[y][x],ScreenMem12,3);
   DrawTile(x,y,backMap[y][x],ScreenMem10,2);
	 DrawTile(x,y,currentMap[y+Map.y][x+Map.x],ScreenMem6,1);
   }
     
  options();
  
  end=1;
  textWindow("Level 1");
  textWindow("Destroy all Brain Cells");
  
  loadMap(1);
  
//Assign the starting values
	guy.x=32;
  guy.y=40;
  guy.lor=1;
  Map.w=800;
  Map.h=800;
  
  baddy[0].x=monsterstack[0][currentstack1]<<3; 
	baddy[0].y=monsterstack[1][currentstack1]<<3;
	currentstack1++;
  baddy[0].w=16;
  baddy[0].h=8;
  

  
	baddy[1].x=monsterstack[0][currentstack2]<<3; 
	baddy[1].y=monsterstack[1][currentstack2]<<3;
	currentstack2++;
  baddy[1].w=16;
  baddy[1].h=8;
  
  BoyScoutOpenSong((unsigned char *)boss);

	// Play song and loop
	BoyScoutPlaySong(1);
   
	//Main Loop
	while(end)
	{	
  
	 KeyCheck(); //check for the keys and move main character
	 
   mapBoundery(); //make sure x and y on the map are possitive
  
   movebaddys(); //move the badguys
   
   movebullets(); //move bullets and check collision
   
   updateTime(DOWN);  //increase the time
   
   WriteText(20, 30, 2, 20, "KILLS:", 4, 0, 0);
   WriteNum(26, 30, 2, 20,kills, 4, 0, 0);
   
   
   
   if(min==0&&sec==0&&millisec==0)  //if time runs out
   {
	textWindow("Time is up");
	
  scroll(UP);
  WriteText(3, 30, 17, 20, "Total Brains Missed:", 4, 0, 0);
  WriteNum( 25, 30, 17, 20, 20-kills, 4, 0, 0);
  WaitKey(KEY_A);
  WriteText(3, 30, 17, 20, "                        ", 4, 0, 0);
  scroll(DOWN);
   }
   
   

   //do stuff with the collisions	  
	if(guyCollision(0)&&dimension==1)  //if touch the brains
	{
	 guy.x=32;guy.y=40; Map.xoffset=0;Map.yoffset=0;
	 textWindow("Dont touch the brains!");
	}
	
		if(guyCollision(1)&&dimension==2)  //if touch the brains
	{
	 guy.x=32;guy.y=40; Map.xoffset=0;Map.yoffset=0;
	 textWindow("Dont touch the brains!");
	}
	
	if(!checkMapCollision(guy.x+4,guy.y+4,16,32)&&!checkMapCollision(guy.x+7,guy.y+4,8,32)) // if the guy inside a wall
	{
	 guy.x=32;guy.y=40; Map.xoffset=0;Map.yoffset=0;
	 textWindow("Your matter dispersed");
  }
	 
	 
  if(currentstack1<=10 && dimension==1)
	if(bulletCollision(0,currentBullet))
	{
		
		if(currentstack1<10)
		{
		baddy[0].x=monsterstack[0][currentstack1]<<3; 
		baddy[0].y=monsterstack[1][currentstack1]<<3;
	  }
	  else
	  {
		baddy[0].x=Map.w;
		baddy[0].y=Map.h;
	  }
	  kills++;
	  currentstack1++;
	  
	}	
	
	 if(currentstack2<=20 && dimension==2)
		if(bulletCollision(1,currentBullet))
	{
		
		if(currentstack2<20)
		{
		baddy[1].x=monsterstack[0][currentstack2]<<3; 
		baddy[1].y=monsterstack[1][currentstack2]<<3;
	  }
	  else
	  {
		baddy[1].x=Map.w;
		baddy[1].y=Map.h;
	  }
	  kills++;
	  currentstack2++;
	  
	}	
	
	
     //draw main guy sprite graphics
     MoveSprite(&sprites[0],guy.x-Map.xoffset,guy.y-Map.yoffset);
     
     //draw enemies if inside sceen
     for(x=0;x<2;x++)
     {
	   if(insideScreen(baddy[x])&&dimension==x+1) //if inside screen draw it else draw outside the screen
 	     MoveSprite(&sprites[5+x],baddy[x].x-Map.xoffset,baddy[x].y-Map.yoffset);
	   else MoveSprite(&sprites[5+x],240,160);
     }
     
     for(x=0;x<3;x++)//draw the bullets if inside sceen 
     {    
       if((bullet[x].y+24>Map.yoffset)&&(bullet[x].y<Map.yoffset+160)&&(bullet[x].x+16>Map.xoffset)&&(bullet[x].x<Map.xoffset+240))//bullet inside sceen?
       MoveSprite(&sprites[2+x],bullet[x].x-Map.xoffset,bullet[x].y-Map.yoffset);
       else MoveSprite(&sprites[2+x],240,160);   
     }
          
   //move the screen

   Map.y=Map.yoffset>>3;//(Map.yoffset/8)
   Map.x=Map.xoffset>>3;

   REG_BG1HOFS=Map.xoffset &7;
   REG_BG1VOFS=Map.yoffset &7;
   
   mapUpdate(); //update the maps

   CopyOAM();   //copy all sprites to screen
   WaitForVSync();  //wait to finish drawing on the sceen
   UpdateDirectSoundA();
   BoyScoutUpdateSong();
   //WaitTime(0,20);
      
	}
	
	return 0;
}

/////////////////////Setup//////////////////////
void Setup(void)
{
	u8 x,y;
	
	//Enable timers
	REG_TM1CNT = FREQUENCY_256 | TIMER_ENABLE;
	
	//Background 0 = Character Memory 0, Screen Memory 8, 16 Color
	//dont use 0,8,16,24 for sceen memory unless using single layer, they overlap charMem.
	REG_BG0CNT = CHAR_MEM(0) | SCREEN_MEM( 4) | BG_COLOR_16 | 0x01;
	REG_BG1CNT = CHAR_MEM(1) | SCREEN_MEM( 6) | BG_COLOR_16 | 0x03;
	REG_BG2CNT = CHAR_MEM(2) | SCREEN_MEM(10) | BG_COLOR_16 | 0x03;
	REG_BG3CNT = CHAR_MEM(3) | SCREEN_MEM(12) | BG_COLOR_16 | 0x01;

	//Mode 0, Background 0
	SET_MODE(MODE_0 | BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | BG3_ENABLE | OBJ_ENABLE | OBJ_MAP_1D);
	
	//Load the text graphics and palette
	 LoadBackgroundTiles((u16*)lettersData, 0, 2688);
	 LoadBackgroundPalette16((u16*)lettersPalette,0); 
	
	 LoadBackgroundTiles((u16*)shapesData,1,640);
   LoadBackgroundPalette16((u16*)shapesPalette,1);

   LoadBackgroundTiles((u16*)backgroundData,2,384);
   LoadBackgroundPalette16((u16*)backgroundPalette,2);
   
   LoadBackgroundTiles((u16*)textmapData,3,512);
   LoadBackgroundPalette16((u16*)textmapPalette,3);
   
   
   //load sprites
   TileMode_LoadSpriteTiles((u16*)guyData,0,1920);
   LoadSpritePalette16((u16*)guyPalette,0);
   
   TileMode_LoadSpriteTiles((u16*)armData,60,64);
  
   TileMode_LoadSpriteTiles((u16*)gunData,62,80);
   
   
   TileMode_LoadSpriteTiles((u16*)bloobData,65,64);
   LoadSpritePalette16((u16*)bloobPalette,2);
   
   TileMode_LoadSpriteTiles((u16*)snakeData,67,256);
   LoadSpritePalette16((u16*)snakePalette,3);
   
	//Hide unused sprites
	InitializeSprites();
	CopyOAM();
	
	 //the main characters body// sprite 0
   sprites[0].attribute0 = COLOR_16 | TALL | MODE_TRANSPERANT | guy.y;
   sprites[0].attribute1 = guy.x | SIZE_32;
   sprites[0].attribute2 = 0 |SPRIORITY;
   
   //main characters arm // sprite 1
   sprites[1].attribute0 = COLOR_16 | TALL | MODE_TRANSPERANT | 160;
   sprites[1].attribute1 = SIZE_8|240;
   
   //main characters ammunition
   for(x=0;x<3;x++) //sprites 2,3,4
   {
   sprites[2+x].attribute0 = COLOR_16 | SQUARE | MODE_TRANSPERANT |0;
   sprites[2+x].attribute1 = 0 | SIZE_8;
   sprites[2+x].attribute2 = 128 |1<<12;
   }
   
   sprites[5].attribute0 = COLOR_16 | WIDE | MODE_TRANSPERANT | 0;
   sprites[5].attribute1 = 0 | SIZE_8;
   sprites[5].attribute2 = 130 |2<<12;
   
   sprites[6].attribute0 = COLOR_16 | WIDE | MODE_TRANSPERANT | 160;
   sprites[6].attribute1 = 240 | SIZE_8;
   sprites[6].attribute2 = 130 |2<<12;
   
	//Clear background
	//FillBackgroundMap(8, 32, 32, 0);
}

/////////////////////Setup Music//////////////////////
void SetupMusic(void)
{
	unsigned int nBSSongSize;  //for sound
  unsigned int nBSSongSize2;  //for sound
  unsigned int nBSSongSize3;
  unsigned int nBSSongSize4;
  
  // Initialize BoyScout
   BoyScoutInitialize();

	// Get needed song memory
	nBSSongSize = BoyScoutGetNeededSongMemory((unsigned char *)tune);
  nBSSongSize2 = BoyScoutGetNeededSongMemory((unsigned char *)boss);
  nBSSongSize3 = BoyScoutGetNeededSongMemory((unsigned char *)rock);
  nBSSongSize4 = BoyScoutGetNeededSongMemory((unsigned char *)music);

	// Allocate and set BoyScout memory area
	BoyScoutSetMemoryArea((unsigned int)malloc(nBSSongSize));
  BoyScoutSetMemoryArea((unsigned int)malloc(nBSSongSize2));
  BoyScoutSetMemoryArea((unsigned int)malloc(nBSSongSize3));
  BoyScoutSetMemoryArea((unsigned int)malloc(nBSSongSize4));

	// Open song
	BoyScoutOpenSong((unsigned char *)rock);

	BoyScoutMuteChannel1(0);
	BoyScoutMuteChannel2(0);
	BoyScoutMuteChannel3(0);
	BoyScoutMuteChannel4(0);

	// Play song and loop
	//BoyScoutPlaySong(1);
  //BoyScoutDecSpeed(1);
}

//******************************************************************************
void mapUpdate(void)
{
	u8 x,y;
  for(y=0; y<22; y++)
	for(x=0; x<32; x++)
	 {
	 //DrawTile(x,y,frontMap[y][x],ScreenMem12,3);
   //DrawTile(x,y,backMap[y][x],ScreenMem10,2);
	 DrawTile(x,y,currentMap[y+Map.y][x+Map.x],ScreenMem6,1);
   }
}



//******************************************************************************
void KeyCheck(void)
{
		
	u8 x,y;
	u8 paused=1;
	count++;
	if (count==20)
	count=0;
	
	if(count>10)  //move the sprite make it seem like he is breathing.
	sprites[0].attribute2 = 0 |SPRIORITY;
	else
	sprites[0].attribute2 = 8 |SPRIORITY;
	
	MoveSprite(&sprites[1],240,160); //set the arm to offscreen
	//MoveSprite(&sprites[2],240,160); //move the bullet[1] offscreen

	
		  //move down 
	  for(x=0;x<3;x++)
	   if(checkMapCollision(guy.x, guy.y+8,16,32)&&checkMapCollision(guy.x+7, guy.y+8,16,32))
      {
       if(guy.y>=76)       //(240-8)/2 sceenw - pixelwidht/2
       Map.yoffset++;
       guy.y++;
       sprites[0].attribute2 =64|SPRIORITY;
      }
      else {guy.jump=0;}
    
    // 
	
     if (KEY_IS(KEY_UP) )
     { 

     }

     if (KEY_IS(KEY_DOWN) )
     {
     sprites[0].attribute2 = 48|SPRIORITY;  
     }

    if ((KEY_IS(KEY_LEFT) )&&(KEY_REL(KEY_L) ))
     {
	    guy.lor=0;
	    sprites[0].attribute1 =guy.x | SIZE_32 | BIT12; //flip sprite horizontaly
	    sprites[1].attribute1 =240 |SIZE_8|BIT12; 
	    
	    if(sprites[0].attribute2!=64)
	    {
	    if(count<7)                                    //walking animation
	    sprites[0].attribute2 =24|SPRIORITY;
	    else if(count<14)
	    sprites[0].attribute2 =32|SPRIORITY;
	    else
	    sprites[0].attribute2 =40|SPRIORITY;
      }
	    
	    for(x=0;x<2;x++)
      if(checkMapCollision(guy.x-1, guy.y+7,16,32))
      {
       if(guy.x<=Map.w-124)  //(240+8)/2 sceenw + pixelwidht/2
       Map.xoffset--;
       guy.x--;
      }
      if (KEY_IS(KEY_B) )//if b held down run
      if(checkMapCollision(guy.x-1, guy.y+7,16,32))
      {
       if(guy.x<=Map.w-124)  //(240+8)/2 sceenw + pixelwidht/2
       Map.xoffset--;
       guy.x--;
      }
     }

    if ((KEY_IS(KEY_RIGHT) )&&(KEY_REL(KEY_L) ))
     {
	    guy.lor=1;
	    sprites[0].attribute1 =guy.x | SIZE_32; //flip sprite horizontaly
	    sprites[1].attribute1 =240 |SIZE_8;
	    
	    if(sprites[0].attribute2!=64)
	    {
	    if(count<7)                                    //walking animation
	    sprites[0].attribute2 =24|SPRIORITY;
	    else if(count<14)
	    sprites[0].attribute2 =32|SPRIORITY;
	    else
	    sprites[0].attribute2 =40|SPRIORITY;
      }
	    
      for(x=0;x<2;x++)
      if(checkMapCollision(guy.x+8, guy.y+7,16,32))
      {
       if(guy.x>=116)       //(240-8)/2 sceenw - pixelwidht/2
       Map.xoffset++;
       guy.x++;
      }
      if (KEY_IS(KEY_B) )  //if b held down run
      if(checkMapCollision(guy.x+8, guy.y+7,16,32))
      {
       if(guy.x>=116)       //(240-8)/2 sceenw - pixelwidht/2
       Map.xoffset++;
       guy.x++;
      }
     }

   
    if (KEY_IS(KEY_A) )
     {  

		 if(guy.jump==0)
		 {
		  guy.jumpAmount=5;
		  if(guy.canjump!=0)
		  PlayDirectSoundA((u8*)JUMP_DATA,JUMP_SAMPRATE,JUMP_LENGTH/FREQ);
	   }
       else if(guy.jump!=0)
       {
		 if(guy.jump<20)
		  guy.jumpAmount=4;
		 else if(guy.jump<40)
		  guy.jumpAmount=3;
		 else if(guy.jump<65)
		  guy.jumpAmount=2;
		 else if(guy.jump<80)
		   guy.canjump=0;
     }

	     
     for(x=0;x<guy.jumpAmount;x++)
       if((guy.jump<100)&&(guy.canjump==1)&&checkMapCollision(guy.x, guy.y-2,16,32)&&checkMapCollision(guy.x+7, guy.y-2,16,32))
       {
	       
	    if(guy.jump<20)
	    sprites[0].attribute2 =56|SPRIORITY;
		  else
		  sprites[0].attribute2 =64|SPRIORITY;	       
	       
       if(guy.y<=Map.h-84)  //(160+8)/2  screenh + pixelh /2
       Map.yoffset-=2;
       guy.y-=2; 
       guy.jump++;
         }
        else {guy.canjump=0;} //bump againts the top cantjump again
   
     }
     
     if(KEY_REL(KEY_A) )
     {
      guy.jump=100;  //to fly make guy.jump =10;
      guy.canjump=1;
     }
     
     mapBoundery(); //make sure map x and y are not negative
     
     if (KEY_IS(KEY_B) ) //when shooting ************
     {
	    
	   if (KEY_IS(KEY_DOWN) )
	   {
		 if(guy.shoot<5)
		 {
     sprites[0].attribute2 =88|SPRIORITY;
     guy.shoot++;
     sprites[1].attribute2 =120|SPRIORITY;  //for the arm
     
     if(guy.lor==0)
     MoveSprite(&sprites[1],guy.x-Map.xoffset-8,guy.y-Map.yoffset+16);
     else if(guy.lor==1)
     MoveSprite(&sprites[1],guy.x-Map.xoffset+16,guy.y-Map.yoffset+16);

     }
	   else if(guy.shoot<10)
	   {
		 
		 if(guy.shoot==5)
		 {
     ShootBullet();
     bullet[currentBullet].y+=1; 
	   }  
		   
	   sprites[0].attribute2 =96|SPRIORITY;
	   guy.shoot++;  
	   sprites[1].attribute2 =122|SPRIORITY;  //for the arm
	   
	   if(guy.lor==0)
     MoveSprite(&sprites[1],guy.x-Map.xoffset-8,guy.y-Map.yoffset+12);
     else if(guy.lor==1)
     MoveSprite(&sprites[1],guy.x-Map.xoffset+16,guy.y-Map.yoffset+12); 
     
     } 
	   }
	   else if(sprites[0].attribute2==64) //in the air
	    {
		 if(guy.shoot<5)
		  {
     sprites[0].attribute2 =104|SPRIORITY;
     guy.shoot++;
     sprites[1].attribute2 =120|SPRIORITY;  //for the arm
     if(guy.lor==0)
     MoveSprite(&sprites[1],guy.x-Map.xoffset-8,guy.y-Map.yoffset+8);//+8
     else if(guy.lor==1)
     MoveSprite(&sprites[1],guy.x-Map.xoffset+16,guy.y-Map.yoffset+8);
      }
     else if(guy.shoot<10)
     {
	     
		 if(guy.shoot==5)
		 {
     ShootBullet();
     bullet[currentBullet].y-=8;
	   } 	   
	       
	   sprites[0].attribute2 =112|SPRIORITY;   
	   guy.shoot++;  
	   sprites[1].attribute2 =122|SPRIORITY;  //for the arm
	   if(guy.lor==0)
     MoveSprite(&sprites[1],guy.x-Map.xoffset-8,guy.y-Map.yoffset+4);//+4
     else if(guy.lor==1)
     MoveSprite(&sprites[1],guy.x-Map.xoffset+16,guy.y-Map.yoffset+4);
     } 
		  }     
	   else  
	     {
	   if(guy.shoot<5)
	    {
     sprites[0].attribute2 =72|SPRIORITY;
     guy.shoot++;
     sprites[1].attribute2 =120|SPRIORITY;  //for the arm
     if(guy.lor==0)
     MoveSprite(&sprites[1],guy.x-Map.xoffset-8,guy.y-Map.yoffset+8);
     else if(guy.lor==1)
     MoveSprite(&sprites[1],guy.x-Map.xoffset+16,guy.y-Map.yoffset+8);
      }
	   else if(guy.shoot<10)
	    {
		    
		 if(guy.shoot==5)
		 {
     ShootBullet();
     bullet[currentBullet].y-=8;
	   } 
		    
		    
	   sprites[0].attribute2 =80|SPRIORITY;  
	   guy.shoot++;  
	   
	   sprites[1].attribute2 =122|SPRIORITY;  //for the arm
	   if(guy.lor==0)
     MoveSprite(&sprites[1],guy.x-Map.xoffset-8,guy.y-Map.yoffset+4);
     else if(guy.lor==1)
     MoveSprite(&sprites[1],guy.x-Map.xoffset+16,guy.y-Map.yoffset+4);
      }  
     }
     }
    if(KEY_REL(KEY_B) ) 
     guy.shoot=0;

    if (KEY_IS(KEY_R) )
     {
	     if(dimension==2)
	     dimension=1;
	     else
	     dimension=2;
      loadMap(dimension);
     }

    if (KEY_IS(KEY_L) ) //used to look around
     {
	     if(bL==0)
	     scroll(UP);
       WriteText(3, 30, 17, 20, "Scan screen with D-pad", 4, 0, 0);
     
     
            if (KEY_IS(KEY_UP) )
         {
	          if(guy.y-Map.yoffset+16<160)
	          Map.yoffset--; 
         }
           if (KEY_IS(KEY_DOWN) )
         {
	          if(guy.y-Map.yoffset>0)
	          Map.yoffset++; 
         }
         
           if (KEY_IS(KEY_LEFT) )
         {
	         if(guy.x-Map.xoffset+16<240)
	          Map.xoffset--; 
         }
           if (KEY_IS(KEY_RIGHT) )
         {
	         if(guy.x-Map.xoffset>0)
	          Map.xoffset++; 
         }
         bL=1;
     }
     
    if(KEY_REL(KEY_L) )
      {      
	     if(bL==1)
	     {
	     WriteText(0, 30, 17, 20, "                             ", 4, 0, 0);
	     scroll(DOWN);
       }
	     Map.xoffset=guy.x-124;
	     Map.yoffset=guy.y-84; 
	     bL=0; 
      } 

    if(KEY_IS (KEY_SELECT) )
    {
	    
	  }
    
	  if(KEY_IS (KEY_START) )
    {
	  scroll(UP);
    WriteText(12, 30, 17, 20, "PAUSED", 4, 0, 0);
    
    for(x=0;x<25;x++)
    {
    WaitTime(0,10);
    UpdateDirectSoundA(); 
    }
    
   while(paused)
     {
	    UpdateDirectSoundA();
	    if(KEY_IS (KEY_START) )
	     {
	     paused=0;
       WaitTime(0,250);
	     }
	   }
	   WriteText(12, 30, 17, 20, "      ", 4, 0, 0);
	   scroll(DOWN);
	  } 	   
}

//**************************************************************************
	  void ShootBullet()
	  {
		 PlayDirectSoundA((u8*)FIRE_DATA,FIRE_SAMPRATE,FIRE_LENGTH/FREQ);
		 currentBullet+=1;
		 if(currentBullet==3)
		 {currentBullet=0;} 
		  
		 sprites[2+currentBullet].attribute0 = COLOR_16 | WIDE | MODE_TRANSPERANT;
		 sprites[2+currentBullet].attribute2 = 125 | SPRIORITY;
		 bullet[currentBullet].x=guy.x;
		 bullet[currentBullet].y=guy.y+16;  
		 if(guy.lor==0)
		 {
		 bullet[currentBullet].velocity=-5;
		 sprites[2+currentBullet].attribute1 = 240 | SIZE_8 ;
		 }
     else if(guy.lor==1)
     {
		 bullet[currentBullet].velocity=5;	  
		 sprites[2+currentBullet].attribute1 = 240 | SIZE_8| BIT12; 
		 }
	  }
	  
	  
//**************************************************************************	  
u8 checkMapCollision(u16 x, u16 y, u8 w, u8 h)
{
u8 b,a;	
u8 temp=1;
	
x=x>>3;
y=y>>3;
w=w>>3;
h=h>>3;

for(b=y;b<y+h;b++)
 for(a=x;a<x+w;a++)
   if(currentMap[b][a]!=0)
    temp=0;
   
   return temp;	
		
}
//**************************************************************************	 
void mapBoundery()
{
	    if(Map.yoffset<0) 
     Map.yoffset=0;
    if(Map.xoffset<0)
     Map.xoffset=0;
    if(Map.xoffset>Map.w-240)
     Map.xoffset=Map.w-240;
    if(Map.yoffset>Map.h-160)
     Map.yoffset=Map.h-160;
}
//**************************************************************************	 
u8 insideScreen(struct badguy b)
{
 	if((b.y+b.h>Map.yoffset)&&(b.y<Map.yoffset+160)&&(b.x+b.w>Map.xoffset)&&(b.x<Map.xoffset+240))
 	return 1;
 	return 0;
}

//**************************************************************************	
u8 guyCollision(u8 b)
{
  return SpriteCollision( baddy[b].x , baddy[b].y , baddy[b].x+baddy[0].w , baddy[b].y+baddy[b].h , guy.x , guy.y, guy.x+16, guy.y+32);
}

//**************************************************************************	
u8 bulletCollision(u8 b, u8 s)
{
	return SpriteCollision( baddy[b].x , baddy[b].y , baddy[b].x+baddy[b].w , baddy[b].y+baddy[b].h , bullet[s].x , bullet[s].y, bullet[s].x+16, bullet[s].y+8);
}

//******************************************************************************
void DoIntro()
{
	u8 x, y;
	
	SET_MODE(MODE_4|BG2_ENABLE );
  
	FadeOut(1);
	
	//compo logo, take off later
	  LoadBackgroundPalette256((u16*)gbaxPalette);

	for(y = 0; y < 160; y++)
	{
		for(x = 0; x < 240; x++)

		{
			PlotPixel(x,y,gbaxData[y*240+x]);//logoData contains the color values of your pict

		}
	}
	WaitForVSync();
	FadeIn(50);
	WaitTime(2,0);
  FadeOut(50);
	
  //madman logo
	LoadBackgroundPalette256((u16*)logoPalette);

	for(y = 0; y < 160; y++)
	{
		for(x = 0; x < 240; x++)

		{
			PlotPixel(x,y,logoData[y*240+x]);//logoData contains the color values of your pict

		}
	}
	WaitForVSync();
	FadeIn(50);
	WaitTime(2,0);
  FadeOut(50);
  
  
  //do the megatroid logo
  LoadBackgroundPalette256((u16*)megatroidPalette);

	for(y = 0; y < 160; y++)
	{
		for(x = 0; x < 240; x++)

		{
			PlotPixel(x,y,megatroidData[y*240+x]);//logoData contains the color values of your pict

		}
	}
	WaitForVSync();
	FadeIn(10);
	
	// Play song and loop
	BoyScoutPlaySong(1);
	PlayDirectSoundA((u8*)MEGA_DATA,MEGA_SAMPRATE,(MEGA_LENGTH)/(FREQ*3));
	
	
	WaitTime(1,0);
	
		while(1)
	{
		if(!(*KEYS & KEY_A))
		{
			break;
		}
		WaitTime(0,16);
		UpdateDirectSoundA();
		BoyScoutUpdateSong();
		
	}
	
  FadeOut(50);
  
  
  ClearBackgroundPalette();
  for(x=0;x<32;x++)
  FillBackgroundMap(x, 32, 32, 0);
  WaitForVSync();
  
  
}
//******************************************************************************
void scroll(u8 d)
{
s8 x;

if(d==DOWN)
 for(x=0;x<30;x++)
 {
 REG_BG3VOFS= -x;
 WaitTime(0,16);
 UpdateDirectSoundA();
 
 BoyScoutUpdateSong();
 }	
else if(d==UP)
 for(x=30;x>-2;x--)
 {
 REG_BG3VOFS= -x;
 WaitTime(0,16);
 UpdateDirectSoundA();
 BoyScoutUpdateSong();
 }
}
//******************************************************************************
void textWindow(char *message)
{
	scroll(UP);
	WriteText(3, 30, 17, 20, message, 4, 0, 0);
  WaitTime(2,0);
  //WaitKey(KEY_A);
  WriteText(0, 30, 17, 20, "                             ", 4, 0, 0);
  scroll(DOWN);
}

//******************************************************************************
void movebaddys()
{
 u8 x, y;	
	
	//for bad guys        

	
	for(y=0;y<2;y++) //number of badguys
	{
  if(count>10)  //animate the brains.
	sprites[5+y].attribute2 = 130 |2<<12 |SPRIORITY;
	else
	sprites[5+y].attribute2 = 132 |2<<12 |SPRIORITY;
  }
		
    
    if(dimension==1)
		for(x=0;x<3;x++)
		{
	   if(checkMapCollision(baddy[0].x, baddy[0].y+8,baddy[0].w,baddy[0].h)&&checkMapCollision(baddy[0].x+7, baddy[0].y+8,baddy[0].w,baddy[0].h))
       baddy[0].y++;
       
    if(checkMapCollision(baddy[0].x-1, baddy[0].y,baddy[0].w,baddy[0].h) && baddy[0].lor==0)  //check on the left
     {
	     if(count==5) 
       baddy[0].x--; 
   
     }
    else 
      baddy[0].lor=1;
    
    if(checkMapCollision(baddy[0].x+8, baddy[0].y,baddy[0].w,baddy[0].h)&& baddy[0].lor==1)  //check to the right
     {
	     if(count==5) 
       baddy[0].x++; 
     }     
    else
      baddy[0].lor=0;
    }
    
   else //for the brain on other dimension
   for(x=0;x<3;x++)
		{
	   if(checkMapCollision(baddy[1].x, baddy[1].y+8,baddy[1].w,baddy[1].h)&&checkMapCollision(baddy[1].x+7, baddy[1].y+8,baddy[1].w,baddy[1].h))
       baddy[1].y++;
       
    if(checkMapCollision(baddy[1].x-1, baddy[1].y,baddy[1].w,baddy[1].h) && baddy[1].lor==0)  //check on the left
     {
	     if(count==5) 
       baddy[1].x--; 
   
     }
    else 
      baddy[1].lor=1;
    
    if(checkMapCollision(baddy[1].x+8, baddy[1].y,baddy[1].w,baddy[1].h)&& baddy[1].lor==1)  //check to the right
     {
	     if(count==5) 
       baddy[1].x++; 
     }     
    else
      baddy[1].lor=0;
    }
  
    
}

//******************************************************************************
void movebullets()
{
	u8 x;
	     //move bullets 
     for(x=0;x<3;x++)
     {    
       bullet[x].x+=bullet[x].velocity;
     
    
       if(bullet[x].x<-16||bullet[x].x>Map.w||bullet[x].y<-8||bullet[x].y>Map.h)
         bullet[x].velocity=0;    
         
       if(!checkMapCollision(bullet[x].x, bullet[x].y,24,8)) //bullets hit the map
        {
	         if(bullet[x].velocity<0)
             sprites[2+x].attribute0 = COLOR_16 | SQUARE | MODE_TRANSPERANT; 
         else if(bullet[x].velocity>0) 
         {
	          bullet[x].x+=8;
            sprites[2+x].attribute0 = COLOR_16 | SQUARE | MODE_TRANSPERANT; 
         }
         bullet[x].velocity=0; 
        } 
      }
	
}
//******************************************************************************
void updateTime(u8 d)
{
//handle time

if(d==UP)
  {
   millisec++;
   if(millisec==60)
    {
	    millisec=0;
	    sec++;
    }
   if(sec==60)
    {
	    sec=0;
	    min++;
    }
  }
else
  {
	 millisec--;
   if(millisec<0)
    {
	    millisec=59;
	    sec--;
    }
   if(sec<0)
    {
	    sec=59;
	    min--;
    }
  }  
  
    WriteText(0, 30, 2, 20, "                             ", 4, 0, 0);
    if(min>=10)
    WriteNum(3, 30, 2, 20, min, 4, 0, 0);
    else{
	  WriteText(3, 30, 2, 20, "0", 4, 0, 0);  
    WriteNum(4, 30, 2, 20, min, 4, 0, 0);}
    
    if(sec>=10)
    WriteNum(6, 30, 2, 20, sec, 4, 0, 0);
    else{
	  WriteText(6, 30, 2, 20, "0", 4, 0, 0);  
    WriteNum(7, 30, 2, 20, sec, 4, 0, 0);}
    
    if(millisec>=10)
    WriteNum(9, 30, 2, 20, millisec, 4, 0, 0);
    else{
	  WriteText(9, 30, 2, 20, "0", 4, 0, 0);  
    WriteNum(10, 30, 2, 20, millisec, 4, 0, 0);}
    
    WriteText(5, 30, 2, 20, ":", 4, 0, 0);
    WriteText(8, 30, 2, 20, ":", 4, 0, 0);	
}

//******************************************************************************
void loadMap(u8 m)
{
 u8 x,y;	
 
 PlayDirectSoundA((u8*)SWAP_DATA,SWAP_SAMPRATE,(SWAP_LENGTH)/(FREQ*10));
 
 FadeOut(50);
	
 for(x=0;x<100;x++)
  for(y=0;y<100;y++)
   switch(m)
   {
   case 1:
   currentMap[x][y]=map1[x][y];
   break;
   case 2:
   currentMap[x][y]=map2[x][y];
   break;
  }
  
  for(x=0;x<3;x++)
  {
  bullet[x].x=Map.w;
  bullet[x].y=Map.h;
  }
  
 mapUpdate();
 FadeIn(50); 
   
}

//******************************************************************************
void options(void)
{
	
	u8 option=0;
	u8 optionch;
	
	FadeIn(1); 
  scroll(UP);	
	
while(end)
  {
	  		if(optionch==1)
		 {
	  
	  scroll(UP);
	  optionch=0;
		 } 
	  
	  switch(option)
	    {
	  case 0:WriteText(3, 30, 17, 20, "Play Game: <Up/Down>/<A>", 4, 0, 0);
       break;
    case 1:WriteText(3, 30, 17, 20, "How To Play: <A> ", 4, 0, 0);  
       break;
    case 2:WriteText(3, 30, 17, 20, "Credits: <A>", 4, 0, 0);  
       break;
    case 3:WriteText(3, 30, 17, 20, "Difficulty:<L/R>", 4, 0, 0); 
       break;
      }
      
    if(option==3)
	  switch(difficulty)
	    {
	  case 0:WriteText(20, 30, 17, 20, "Normal", 4, 0, 0);
       break;
    case 1:WriteText(20, 30, 17, 20, "Novice", 4, 0, 0);  
       break;
    case 2:WriteText(20, 30, 17, 20, "Hard  ", 4, 0, 0);  
       break;
      }
       
  
       if (KEY_IS(KEY_UP) || KEY_IS(KEY_DOWN))
         {
	          WriteText(3, 30, 17, 20, "                        ", 4, 0, 0);
	          option++;
	          if(option==4)
	          option=0;
	          optionch=1;
	          scroll(DOWN);
         }
         
       if(KEY_IS(KEY_LEFT)|| KEY_IS(KEY_RIGHT)&&option==3)
         {
	         difficulty++;
	         if(difficulty==3)
	         difficulty=0;
	         WaitTime(0,300);
         }  
         

        
       if(KEY_IS(KEY_A)&&option==1)
        {
	       WriteText(3, 30, 17, 20, "                        ", 4, 0, 0);
	       scroll(DOWN);
	       textWindow("D-pad moves the character");
	       textWindow("B Shoots the bullets");
	       textWindow("R changes dimenssion");
	       textWindow("Hold B to run"); 
	       textWindow("Hold L to move the camera");
	       scroll(UP);
        }
        
       if(KEY_IS(KEY_A)&&option==2)
       {
	       WriteText(3, 30, 17, 20, "                        ", 4, 0, 0);
	       scroll(DOWN);
	       textWindow("Code: Werner Mendizabal");
	       textWindow("Art: Werner Mendizabal");
	       textWindow("Music: Werner Mendizabal");
	       textWindow("Special Thanks:"); 
	       textWindow("http://www.gbaemu.com/");
	       textWindow("http://www.gbadev.org/");
	       textWindow("//www.thepernproject.com/");
	       textWindow("Boy Scout - music engine");

	       scroll(UP); 
       } 
        
       if(KEY_IS(KEY_A)&&option==0)
        {
	       WriteText(3, 30, 17, 20, "                        ", 4, 0, 0);
         end=0;
         scroll(DOWN);
         
     switch(difficulty)
	    {
	  case 0:min=5;
       break;
    case 1:min=10;
       break;
    case 2:min=3; 
       break;
      }
        }
  } 	
}
