#include "gba.h"
#include "time.h"
#include "screenmode.h"
#include "keypad.h"
#include "gfx.h"
#include "timers.h"
#include "gfx/logo.h"
#include "gfx/letters.h"
#include "gfx/background.h"
#include "gfx/target.h"
#include "gfx/intro.h"
#include "gfx/circle.h"
#include "sound/intro.h"
#include "sound/song.h"
#include "sound.h" 

#define MAXROW (10)
#define MAXCOL (10)

#define TIMER_INTERVAL      (0xFFFF - (761*2))
#define FREQ 190

#define GAMEPAK_RAM  ((u8*)0x0E000000)
u8 *pSaveMemory = GAMEPAK_RAM;

//example to save: pSaveMemory[0] = 1;

// some memory location defines
u16* FrontBuffer = (u16*)0x6000000;
u16* BackBuffer = (u16*)0x600A000;
u16* videoBuffer;
volatile u16* ScanlineCounter = (volatile u16*)0x4000006;

u16* paletteMem = (u16*)0x5000000;	//PalMemory is 256 16 bit BGR values starting at 0x5000000

int player1 [MAXROW][MAXCOL]; //front array used for player1
int player2  [MAXROW][MAXCOL]; //back array used for player2
int ship1[6];
int ship2[6];
int turn;
int gamemode;
char name1[10];
int name1lenght;
char name2[10];
int name2lenght;

extern const u8 INTRO_DATA[];  //the sample, 8bit signed, 16Khz

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
void DoIntro2(void);      //does the intro
void WaitForStart(void);	// waits for the start key to be pressed
void WaitForA(void);
void Flip(void);			// flips between the back front buffer
void WaitForVblank(void);	// waits for the drawer to get to end before flip
void FadeOut( u32 aWait ); //fades out
void FadeIn( u32 aWait ); // fades in
void numbers(int numx, int numy,int num); //prints out the numbers white black background
void letters(int numx, int numy, char num); //used to print out the letters
void writetext(int x, int y, char *message); //used to print out the letters
void writetextslow(int x, int y, char *message); //used to print out the letters
void writetextblock(char *message, int sleepamount); //prints out letters onto the whole screen;
void writenumbers(int x, int y, int num); // used to print out numbers
void writenumbersslow(int x, int y, int num); // used to print out numbers
int pow2(int num, int exp);  //raises a number to a certain power... made to work with my program
int random(int min, int max);
void background(void);
void print(void);
void shapes(int numx, int numy, int num);
int main1(void);
int main2(void);
int nameinput(char namesent[]);

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

int main(void)
{
    int x;
    int end;

    SetMode(MODE_4 | BG2_ENABLE );   //set to mode 4

    while(1)
    {
        end=0;


        EraseScreen();
        WaitForVblank();
        EraseScreen();
        WaitForVblank();

        for(x = 0; x < 256; x++)
            paletteMem[x] = logoPalette[x];

        DoIntro();
        PlayDirectSoundA((u8*)INTRO_DATA,INTRO_SAMPRATE,(INTRO_LENGTH)/(INTRO_SAMPRATE));

        Sleep(55);
        PlayDirectSoundA((u8*)SONG_DATA,SONG_SAMPRATE,(SONG_LENGTH)/(SONG_SAMPRATE));

        DoIntro2();

        EraseScreen();
        WaitForVblank();


        x=0;

        writetext(30,65, "One Player Game_");

        writetext(30,80, "Two Player Game_");

        writetext(30,95, "How To Play_");

        writetext(20,65+15*x, "**_");
        writetext(92,65+15*x, "**_");
        FadeIn(1);

        while(end!=1)
        {
            UpdateDirectSoundA();
            EraseScreen();
            WaitForVblank();
            Flip();

            writetext(30,65, "One Player Game_");

            writetext(30,80, "Two Player Game_");

            writetext(30,95, "How To Play_");

            writetext(20,65+15*x, "**_");
            writetext(92,65+15*x, "**_");

            Flip();
            Sleep(5);

            if (! ((*KEYS) & KEY_UP) )
                if (x!=0)
                    x--;

            if (! ((*KEYS) & KEY_DOWN) )
                if (x!=2)
                    x++;

            if (! ((*KEYS) & KEY_A) )
            {
                if (x==0)
                    end=main1();
                if (x==1)
                    end=main2();
                if (x==2)
                {
                    EraseScreen();
                    WaitForVblank();
                    Flip();
                    writetext(5,20,"Use the D-pad to move around,_");
                    writetext(5,30,"B to change the ships,_");
                    writetext(5,40,"A to select options and_");
                    writetext(5,50,"Start to begin players turn_");
                    writetext(5,60,"in two player mode._");
                    Flip();
                    Sleep(20);
                    WaitForA();
                    Sleep(10);
                }
            }
        }
    }
    return 0;
}

//******************************************************************************
int main1 (void)
{
    int x,y;
    int px, py;
    int cx, cy;
    int end;
    int shape;
    int count;
    int player1end, player2end;
    int computercount;
    int cr;
    int aix = 0, aiy = 0, aix2 = 0, aiy2 = 0;
    int hit;

    gamemode=1;

    //start timer
    REG_TM3CNT = FREQUENCY_1024 | TIMER_ENABLE;
    //zero the timer
    REG_TM3D = 0;

    while(1)
    {
        px=0;
        py=0;
        cx=0;
        cy=0;
        end=0;
        shape=0;
        count=1;
        computercount=0;
        hit=0;

        for(y=0; y<10; y++)
            for(x=0; x<10; x++)
            {
                player1[y][x]=0;
                player2[y][x]=0;
            }

        for(y=0; y<6; y++)
        {
            ship1[y]=0;
            ship2[y]=0;
        }

        EraseScreen();
        WaitForVblank();
        EraseScreen();
        WaitForVblank();


        writetext(10,10, "what is your name player1?_");
        Flip();
        FadeIn(1);
        Sleep(100);
        FadeOut(1);
        name1lenght=nameinput(name1);

        EraseScreen();
        WaitForVblank();
        FadeIn(1);

        paletteMem[255]=RGB(31,31,31);


        print();
        Flip();
        FadeIn(1);

        //set up battleships for player1
        while(count!=6)
        {
            turn=1;
            print();

            if(shape==0)
            {
                shapes((px*5)+5,(py*10)+30,8);
                shapes((px*5)+5,(py*10)+30,1);
                shapes(((px+1)*5)+5,(py*10)+30,1);
                shapes(((px+2)*5)+5,(py*10)+30,1);
            }
            else
            {
                shapes((px*5)+5,(py*10)+30,8);
                shapes((px*5)+5,(py*10)+30,1);
                shapes((px*5)+5,((py+1)*10)+30,1);
                shapes((px*5)+5,((py+2)*10)+30,1);
            }

            Flip();

            if (! ((*KEYS) & KEY_B) )
            {
                if (shape==0)
                {
                    if((py==8)||(py==9))
                        shape=0;
                    else
                        shape=1;
                }
                else
                {
                    if((px==8)||(px==9))
                        shape=1;
                    else
                        shape=0;
                }

            }

            if (! ((*KEYS) & KEY_DOWN) )
            {
                if (shape==0)
                {
                    if(py!=9)
                        py++;
                }
                else
                    if(py!=7)
                        py++;

            }

            if (! ((*KEYS) & KEY_UP) )
            {

                if(py!=0)
                    py--;

            }


            if (! ((*KEYS) & KEY_RIGHT) )
            {
                if (shape==0)
                {
                    if(px!=7)
                        px++;
                }
                else
                    if(px!=9)
                        px++;

            }

            if (! ((*KEYS) & KEY_LEFT) )
            {

                if(px!=0)
                    px--;

            }

            if (! ((*KEYS) & KEY_A) )
            {
                if (shape==0)
                {
                    if ((player1[py][px]==0)&&(player1[py][px+1]==0)&&(player1[py][px+2]==0))
                    {
                        player1[py][px]=count;
                        player1[py][px+1]=count;
                        player1[py][px+2]=count;
                        count++;
                    }
                }
                else
                {
                    if ((player1[py][px]==0)&&(player1[py+1][px]==0)&&(player1[py+2][px]==0))
                    {
                        player1[py][px]=count;
                        player1[py+1][px]=count;
                        player1[py+2][px]=count;
                        count++;
                    }

                }
            }


        }


        //set up the battleships for computer

        count=1;
        while(count!=6)
        {
            shape=random(0,1);

            if (shape==0)
            {
                cx=random(0,7);
                cy=random(0,9);
            }
            else
            {
                cx=random(0,9);
                cy=random(0,7);
            }

            if (shape==0)
            {
                if ((player2[cy][cx]==0)&&(player2[cy][cx+1]==0)&&(player2[cy][cx+2]==0))
                {
                    player2[cy][cx]=count;
                    player2[cy][cx+1]=count;
                    player2[cy][cx+2]=count;
                    count++;
                }
            }
            else
            {
                if ((player2[cy][cx]==0)&&(player2[cy+1][cx]==0)&&(player2[cy+2][cx]==0))
                {
                    player2[cy][cx]=count;
                    player2[cy+1][cx]=count;
                    player2[cy+2][cx]=count;
                    count++;
                }
            }

            EraseScreen();
            WaitForVblank();
            Flip();


        }

        print();
        Flip();


        px=0;
        py=0;

        while (end!=1)
        {
            player1end=0;
            player2end=0;

            while(player1end!=1)
            {

                //random number for the computer... done here for more randomnese
                cr=random(0,9);

                //player1 turn
                turn=1;
                print();

                shapes((px*5)+65,(py*10)+30,8);
                Flip();

                if (! ((*KEYS) & KEY_DOWN) )
                {
                    if(py!=9)
                        py++;
                }

                if (! ((*KEYS) & KEY_UP) )
                {
                    if(py!=0)
                        py--;
                }

                if (! ((*KEYS) & KEY_RIGHT) )
                {
                    if(px!=9)
                        px++;
                }

                if (! ((*KEYS) & KEY_LEFT) )
                {
                    if(px!=0)
                        px--;
                }

                if (! ((*KEYS) & KEY_A) )
                {
                    if(player2[py][px]==0)
                    {
                        player1end=1;
                        player2[py][px]=7;
                        print();
                        writetext(35,140,"You Missed!!_");
                        Flip();
                        Sleep(50);
                    }
                    else if((player2[py][px]==6)||(player2[py][px]==7))
                    {

                    }
                    else
                    {
                        player1end=1;
                        ship1[player2[py][px]]++;
                        print();
                        writetext(40,140,"You Hit!!_");
                        Flip();
                        Sleep(5);
                        EraseScreen();
                        WaitForVblank();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(40);


                        if(ship1[player2[py][px]]==3)
                        {
                            ship1[0]++;
                            print();
                            writetext(40,140,"Ship Sunk!!_");
                            Flip();
                            Sleep(50);
                        }
                        player2[py][px]=6;
                    }
                }
                if(ship1[0]==5)
                {
                    end=1;
                    player1end=1;
                    EraseScreen();
                    WaitForVblank();
                    Flip();

                    for(x=0; x<name1lenght; x++)
                        letters((120-(4*(name1lenght+6)))/2+(4*x),70,name1[x]);
                    writetext((120-(4*(name1lenght+6)))/2+(4*name1lenght),70, " wins!_");

                    Flip();
                    Sleep(150);
                    return 1;
                }
            }

            computercount=0;
            cx=cr;
            //computers turn
            while(player2end!=1)
            {
                computercount++;


                if(hit==0)
                {
                    //random if ship is not hit

                    if(computercount>30)
                    {

                        if(cx==0)
                        {
                            cx=9;
                            computercount=0;
                        }
                        else if(cx<10)
                        {
                            cx--;
                            computercount=0;
                        }
                    }
                    cy=random(0,9);
                }

                else if(hit==1)
                {
                    //random if something is hit

                    if(computercount<20)
                    {
                        switch(random(1,4))
                        {
                            case 1:cx=aix-1; cy=aiy;
                                   break;
                            case 2:cx=aix+1; cy=aiy;
                                   break;
                            case 3:cx=aix; cy=aiy-1;
                                   break;
                            case 4:cx=aix; cy=aiy+1;
                                   break;
                        }
                    }
                    else
                    {
                        hit=0;
                    }
                }
                else if (hit==2)
                {

                    if(aix==aix2)
                    {
                        cx=aix;
                        switch(computercount)
                        {
                            case 1: cy=aiy-1;
                                    break;
                            case 2: cy=aiy+1;
                                    break;
                            case 3: cy=aiy2-1;
                                    break;
                            case 4: cy=aiy2+1;
                                    break;
                            case 5: hit=0; computercount=0;
                                    break;
                        }
                    }
                    else if(aiy==aiy2)
                    {
                        cy=aiy;
                        switch(computercount)
                        {
                            case 1: cx=aix-1;
                                    break;
                            case 2: cx=aix+1;
                                    break;
                            case 3: cx=aix2-1;
                                    break;
                            case 4: cx=aix2+1;
                                    break;
                            case 5: hit=0; computercount=0;
                                    break;
                        }
                    }
                }

                if((cx<=9)&&(cx>=0)&&(cy<=9)&&(cy>=0))
                {

                    if(player1[cy][cx]==0)
                    {
                        player2end=1;
                        player1[cy][cx]=7;
                        print();
                        writetext(30,140,"Computer Missed!!_");
                        Flip();
                        Sleep(50);
                    }
                    else if((player1[cy][cx]==6)||(player1[cy][cx]==7))
                    {}
                    else
                    {
                        hit++;
                        if(hit==1)
                        {
                            aix=cx;
                            aiy=cy;
                        }
                        if(hit==2)
                        {
                            aix2=cx;
                            aiy2=cy;
                        }
                        player2end=1;
                        ship2[player1[cy][cx]]++;
                        print();
                        writetext(35,140,"Computer Hit!!_");
                        Flip();
                        Sleep(5);
                        EraseScreen();
                        WaitForVblank();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(40);


                        if(ship2[player1[cy][cx]]==3)
                        {
                            hit=0;
                            ship2[0]++;
                            print();
                            writetext(40,140,"Ship Sunk!!_");
                            Flip();
                            Sleep(50);
                        }
                        player1[cy][cx]=6;
                    }
                }
            }

            if(ship2[0]==5)
            {
                end=1;
                player2end=1;
                EraseScreen();
                WaitForVblank();
                Flip();
                writetext(35,70,"Computer Wins_");
                Flip();
                Sleep(150);
                return 1;
            }

            if (hit==3)
                hit=1;
        }

    }
    return 1;
}

//******************************************************************************
int main2 (void)
{
    int x,y;
    int px, py;
    int cx, cy;
    int end;
    int shape;
    int count;
    int player1end, player2end;
    gamemode=2;

    //start timer
    REG_TM3CNT = FREQUENCY_1024 | TIMER_ENABLE;
    //zero the timer
    REG_TM3D = 0;

    while(1)
    {
        px=0;
        py=0;
        end=0;
        shape=0;
        count=1;

        for(y=0; y<10; y++)
            for(x=0; x<10; x++)
            {
                player1[y][x]=0;
                player2[y][x]=0;
            }

        for(y=0; y<6; y++)
        {
            ship1[y]=0;
            ship2[y]=0;
        }

        EraseScreen();
        WaitForVblank();
        EraseScreen();
        WaitForVblank();

        writetext(10,10, "what is your name player1?_");
        Flip();
        FadeIn(1);
        Sleep(100);
        FadeOut(1);
        name1lenght=nameinput(name1);

        EraseScreen();
        WaitForVblank();
        Flip();
        writetext(10,10, "what is your name player2?_");
        Flip();
        FadeIn(1);
        Sleep(100);
        FadeOut(1);
        name2lenght=nameinput(name2);

        EraseScreen();
        WaitForVblank();
        FadeIn(1);

        paletteMem[255]=RGB(31,31,31);

        print();

        //set up battleships for player1


        EraseScreen();
        WaitForVblank();
        Flip();

        for(x=0; x<name1lenght; x++)
            letters((120-(4*(name1lenght+6)))/2 +(4*x),70,name1[x]); //letters(35+(4*x),70,name1[x]);
        writetext((120-(4*(name1lenght+6)))/2 +(4*name1lenght),70, "s turn_");

        Flip();
        FadeIn(1);
        WaitForStart();
        FadeOut(1);


        turn=1;
        print();
        Flip();
        FadeIn(1);

        while(count!=6)
        {
            print();

            if(shape==0)
            {
                shapes((px*5)+5,(py*10)+30,8);
                shapes((px*5)+5,(py*10)+30,1);
                shapes(((px+1)*5)+5,(py*10)+30,1);
                shapes(((px+2)*5)+5,(py*10)+30,1);
            }
            else
            {
                shapes((px*5)+5,(py*10)+30,8);
                shapes((px*5)+5,(py*10)+30,1);
                shapes((px*5)+5,((py+1)*10)+30,1);
                shapes((px*5)+5,((py+2)*10)+30,1);
            }

            Flip();

            if (! ((*KEYS) & KEY_B) )
            {
                if (shape==0)
                {
                    if((py==8)||(py==9))
                        shape=0;
                    else
                        shape=1;
                }
                else
                {
                    if((px==8)||(px==9))
                        shape=1;
                    else
                        shape=0;
                }

            }

            if (! ((*KEYS) & KEY_DOWN) )
            {
                if (shape==0)
                {
                    if(py!=9)
                        py++;
                }
                else
                    if(py!=7)
                        py++;

            }

            if (! ((*KEYS) & KEY_UP) )
            {

                if(py!=0)
                    py--;

            }


            if (! ((*KEYS) & KEY_RIGHT) )
            {
                if (shape==0)
                {
                    if(px!=7)
                        px++;
                }
                else
                    if(px!=9)
                        px++;

            }

            if (! ((*KEYS) & KEY_LEFT) )
            {

                if(px!=0)
                    px--;

            }

            if (! ((*KEYS) & KEY_A) )
            {
                if (shape==0)
                {
                    if ((player1[py][px]==0)&&(player1[py][px+1]==0)&&(player1[py][px+2]==0))
                    {
                        player1[py][px]=count;
                        player1[py][px+1]=count;
                        player1[py][px+2]=count;
                        count++;
                    }
                }
                else
                {
                    if ((player1[py][px]==0)&&(player1[py+1][px]==0)&&(player1[py+2][px]==0))
                    {
                        player1[py][px]=count;
                        player1[py+1][px]=count;
                        player1[py+2][px]=count;
                        count++;
                    }

                }
            }


        }


        EraseScreen();
        WaitForVblank();
        Flip();

        for(x=0; x<name2lenght; x++)
            letters((120-(4*(name2lenght+6)))/2+(4*x),70,name2[x]);
        writetext((120-(4*(name2lenght+6)))/2+(4*name2lenght),70, "s turn_");

        Flip();
        FadeIn(1);
        WaitForStart();
        FadeOut(1);

        turn=2;
        print();
        Flip();
        FadeIn(1);

        px=0;
        py=0;

        //set up the battleships for player2
        count=1;
        while(count!=6)
        {
            print();

            if(shape==0)
            {
                shapes((px*5)+65,(py*10)+30,8);
                shapes((px*5)+65,(py*10)+30,1);
                shapes(((px+1)*5)+65,(py*10)+30,1);
                shapes(((px+2)*5)+65,(py*10)+30,1);
            }
            else
            {
                shapes((px*5)+65,(py*10)+30,8);
                shapes((px*5)+65,(py*10)+30,1);
                shapes((px*5)+65,((py+1)*10)+30,1);
                shapes((px*5)+65,((py+2)*10)+30,1);
            }

            Flip();

            if (! ((*KEYS) & KEY_B) )
            {
                if (shape==0)
                {
                    if((py==8)||(py==9))
                        shape=0;
                    else
                        shape=1;
                }
                else
                {
                    if((px==8)||(px==9))
                        shape=1;
                    else
                        shape=0;
                }

            }

            if (! ((*KEYS) & KEY_DOWN) )
            {
                if (shape==0)
                {
                    if(py!=9)
                        py++;
                }
                else
                    if(py!=7)
                        py++;

            }

            if (! ((*KEYS) & KEY_UP) )
            {

                if(py!=0)
                    py--;

            }


            if (! ((*KEYS) & KEY_RIGHT) )
            {
                if (shape==0)
                {
                    if(px!=7)
                        px++;
                }
                else
                    if(px!=9)
                        px++;

            }

            if (! ((*KEYS) & KEY_LEFT) )
            {

                if(px!=0)
                    px--;

            }

            if (! ((*KEYS) & KEY_A) )
            {
                if (shape==0)
                {
                    if ((player2[py][px]==0)&&(player2[py][px+1]==0)&&(player2[py][px+2]==0))
                    {
                        player2[py][px]=count;
                        player2[py][px+1]=count;
                        player2[py][px+2]=count;
                        count++;
                    }
                }
                else
                {
                    if ((player2[py][px]==0)&&(player2[py+1][px]==0)&&(player2[py+2][px]==0))
                    {
                        player2[py][px]=count;
                        player2[py+1][px]=count;
                        player2[py+2][px]=count;
                        count++;
                    }

                }
            }


        }
        px=0;
        py=0;
        cx=0;
        cy=0;
        //playing the game
        while (end!=1)
        {
            player1end=0;
            player2end=0;

            EraseScreen();
            WaitForVblank();
            Flip();

            for(x=0; x<name1lenght; x++)
                letters((120-(4*(name1lenght+6)))/2+(4*x),70,name1[x]);
            writetext((120-(4*(name1lenght+6)))/2+(4*name1lenght),70, "s turn_");

            Flip();
            FadeIn(1);
            WaitForStart();
            FadeOut(1);

            turn=1;
            print();
            Flip();
            FadeIn(1);

            while(player1end!=1)
            {
                //player1 turn
                print();

                shapes((px*5)+65,(py*10)+30,8);
                Flip();


                if (! ((*KEYS) & KEY_DOWN) )
                {
                    if(py!=9)
                        py++;
                }

                if (! ((*KEYS) & KEY_UP) )
                {
                    if(py!=0)
                        py--;
                }

                if (! ((*KEYS) & KEY_RIGHT) )
                {
                    if(px!=9)
                        px++;
                }

                if (! ((*KEYS) & KEY_LEFT) )
                {
                    if(px!=0)
                        px--;
                }

                if (! ((*KEYS) & KEY_A) )
                {
                    if(player2[py][px]==0)
                    {
                        player1end=1;
                        player2[py][px]=7;
                        print();
                        writetext(35,140,"You Missed!!_");
                        Flip();
                        Sleep(50);
                    }
                    else if((player2[py][px]==6)||(player2[py][px]==7))
                    {

                    }
                    else
                    {
                        player1end=1;
                        ship1[player2[py][px]]++;
                        print();
                        writetext(40,140,"You Hit!!_");
                        Flip();
                        Sleep(5);
                        EraseScreen();
                        WaitForVblank();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(40);


                        if(ship1[player2[py][px]]==3)
                        {
                            ship1[0]++;
                            print();
                            writetext(40,140,"Ship Sunk!!_");
                            Flip();
                            Sleep(50);
                        }
                        player2[py][px]=6;
                    }
                }
                if(ship1[0]==5)
                {
                    end=1;
                    player1end=1;
                    EraseScreen();
                    WaitForVblank();
                    Flip();

                    for(x=0; x<name1lenght; x++)
                        letters((120-(4*(name1lenght+6)))/2+(4*x),70,name1[x]);
                    writetext((120-(4*(name1lenght+6)))/2+(4*name1lenght),70, " wins!_");

                    Flip();
                    Sleep(150);
                    return 1;
                }
            }


            EraseScreen();
            WaitForVblank();
            Flip();

            for(x=0; x<name2lenght; x++)
                letters((120-(4*(name2lenght+6)))/2+(4*x),70,name2[x]);
            writetext((120-(4*(name2lenght+6)))/2+(4*name2lenght),70, "s turn_");

            Flip();
            FadeIn(1);
            WaitForStart();
            FadeOut(1);

            turn=2;
            print();
            Flip();
            FadeIn(1);

            while(player2end!=1)
            {

                //player2 turn
                print();

                shapes((cx*5)+5,(cy*10)+30,8);
                Flip();


                if (! ((*KEYS) & KEY_DOWN) )
                {
                    if(cy!=9)
                        cy++;
                }

                if (! ((*KEYS) & KEY_UP) )
                {
                    if(cy!=0)
                        cy--;
                }

                if (! ((*KEYS) & KEY_RIGHT) )
                {
                    if(cx!=9)
                        cx++;
                }

                if (! ((*KEYS) & KEY_LEFT) )
                {
                    if(cx!=0)
                        cx--;
                }

                if (! ((*KEYS) & KEY_A) )
                {
                    if(player1[cy][cx]==0)
                    {
                        player2end=1;
                        player1[cy][cx]=7;
                        print();
                        writetext(35,140,"You Missed!!_");
                        Flip();
                        Sleep(50);
                    }
                    else if((player1[cy][cx]==6)||(player1[cy][cx]==7))
                    {

                    }
                    else
                    {
                        player2end=1;
                        ship2[player1[cy][cx]]++;
                        print();
                        writetext(40,140,"You Hit!!_");
                        Flip();
                        Sleep(5);
                        EraseScreen();
                        WaitForVblank();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(5);
                        Flip();
                        Sleep(40);


                        if(ship2[player1[cy][cx]]==3)
                        {
                            ship2[0]++;
                            print();
                            writetext(40,140,"Ship Sunk!!_");
                            Flip();
                            Sleep(50);
                        }
                        player1[cy][cx]=6;
                    }
                }
                if(ship2[0]==5)
                {
                    end=1;
                    player2end=1;
                    EraseScreen();
                    WaitForVblank();
                    Flip();

                    for(x=0; x<name2lenght; x++)
                        letters((120-(4*(name2lenght+6)))/2+(4*x),70,name2[x]);
                    writetext((120-(4*(name2lenght+6)))/2+(4*name2lenght),70, " wins!_");

                    Flip();
                    Sleep(150);
                    return 1;
                }
            }
        }
    }
    return 1;
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
    int x, y, z;

    for(x = 0; x < 256; x++)
        paletteMem[x] = logoPalette[x];	//loop through and store the palette from your pict
    //palette into video palette mem


    for(y = 0; y < 160; y++)
    {
        for(x = 0; x < 120; x++)

        {
            PlotPixel(x,y,logoData[y*360/2+x+60]);//logoData contains the color values of your pict

        }
    }
    WaitForVblank();
    Flip();

    FadeIn(1);
    WaitForA();

    for(z=60; z>=0; z--)
    {
        for(y = 0; y < 160; y++)
        {
            for(x = 0; x < 120; x++)

            {
                PlotPixel(x,y,logoData[y*180+x+z]);//logoData contains the color values of your pict

            }

        }
        WaitForVblank();
        Flip();
        Sleep(1/2);
    }

    Flip();
    writetext(10,60, "MADMAN PRODUCTIONS_");
    writetext(25,80, "-PRESENTS-_");
    Flip();
    FadeIn(1);
    Sleep(120);
    FadeOut(1);
}

//******************************************************************************
void DoIntro2(void)
{
    int x, y,z;

    for(x = 0; x < 256; x++)
        paletteMem[x] = introPalette[x];	//loop through and store the palette from your pict
    //palette into video palette mem

    EraseScreen();
    WaitForVblank();
    FadeIn(1);


    for(z=0; z<20; z++)
    {
        for(y = 0; y < 160; y++)
        {
            for(x = 0; x < 120; x++)

            {
                PlotPixel(x,y,introData[y*120+x]);//logoData contains the color values of your pict

            }
        }


        for(y = 0; y < 50; y++)
        {
            for(x = 0; x < 25; x++)

            {
                PlotPixelTransparent(x+40-z,y+50,circleData[y*25+x]);//logoData contains the color values of your pict
                PlotPixelTransparent(x+40+z,y+50,circleData[y*25+x]);
                PlotPixelTransparent(x+40,y+50-(z*2),circleData[y*25+x]);
                PlotPixelTransparent(x+40,y+50+(z*2),circleData[y*25+x]);
            }
        }

        WaitForVblank();
        Flip();
        Sleep(2);
    }

    Flip();
    writetext(60,110, "By: Werner M_");
    Flip();

    EraseScreen();
    WaitForVblank(); paletteMem[255]=RGB(31,31,0);
    Sleep(5);
    Flip();
    Sleep(10);
    Flip();     paletteMem[255]=RGB(0,31,0);
    Sleep(5);
    Flip();
    Sleep(10);
    Flip();     paletteMem[255]=RGB(0,0,31);
    Sleep(5);
    Flip();
    Sleep(10);
    Flip();     paletteMem[255]=RGB(31,0,0);
    Sleep(5);
    Flip();
    Sleep(10);
    Flip();     paletteMem[255]=RGB(0,31,31);
    Sleep(5);
    Flip();
    Sleep(10);
    Flip();     paletteMem[255]=RGB(31,31,31);
    Sleep(5);
    Flip();
    Sleep(10);


    Sleep(50);
    FadeOut(1);

}

//******************************************************************************
void background(void)
{
    int x, y;
    for(x = 0; x < 256; x++)
        paletteMem[x] = backgroundPalette[x];	//loop through and store the palette from your pict
    //palette into video palette mem

    paletteMem[255] = RGB(31,0,0);  //paletteMem[75];

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
void WaitForStart(void)
{
    while (1) // loops infinitely till they press start
        if (! ((*KEYS) & KEY_START) )
            return;
}

//******************************************************************************
void WaitForA(void)
{
    while (1) // loops infinitely till they press start
    {
        UpdateDirectSoundA();
        if (! ((*KEYS) & KEY_A) )
            return;
    }
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
            PlotPixelTransparent(x+numx, y+numy, lettersData[(y+temp)*(letters_WIDTH/2)+x]);

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
void writenumbers(int x, int y, int num)
{
    int a;
    int b=0;
    int temp=num;
    int place=0;

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
void writenumbersslow(int x, int y, int num)
{
    int a;
    int b=0;
    int temp=num;
    int place=0;

    if (num<10)
    {
        numbers(x,y,num);
        Sleep(2);
        WaitForVblank();
        Flip();
        numbers(x,y,num);
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
            {numbers(x,y,num/pow2(10,a));
                Sleep(2);
                WaitForVblank();
                Flip();
                numbers(x,y,num/pow2(10,a));
                Sleep(2);
                WaitForVblank();
                Flip();
                place++;}

            else if(a==0)
            {numbers(x+(place*4),y,num%10);
                Sleep(2);
                WaitForVblank();
                Flip();
                numbers(x+(place*4),y,num%10);
                Sleep(2);
                WaitForVblank();
                Flip();
                place++;}

            else
            {numbers(x+(place*4),y,(num/pow2(10,a))%10);
                Sleep(2);
                WaitForVblank();
                Flip();
                numbers(x+(place*4),y,(num/pow2(10,a))%10);
                Sleep(2);
                WaitForVblank();
                Flip();

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
            PlotPixel(x+numx, y+numy, lettersData[(y+temp)*(letters_WIDTH/2)+x]);
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
void print(void)
{
    int x,y;

    background();

    for(y=0; y<10; y++)
        for(x=0; x<10; x++)
        {
            if(turn==1)
                shapes((x*5)+5,(y*10)+30,player1[y][x]);
            if ((player2[y][x]==6)||(player2[y][x]==7))
                shapes((x*5)+65,(y*10)+30,player2[y][x]);

            if(turn==2)
                shapes((x*5)+65,(y*10)+30,player2[y][x]);
            if ((player1[y][x]==6)||(player1[y][x]==7))
                shapes((x*5)+5,(y*10)+30,player1[y][x]);
        }

    for(x=0; x<name1lenght; x++)
        letters(15+(4*x),10,name1[x]);

    if (gamemode==1)
        writetext(75,10, "Computer_");
    else
    {
        for(x=0; x<name2lenght; x++)
            letters(75+(4*x),10,name2[x]);
    }

    writenumbers(110,10,ship2[0]);
    writenumbers(50,10,ship1[0]);
}

//******************************************************************************
void shapes(int numx, int numy, int num)
{
    int x, y;
    int temp=0;

    switch(num)
    {
        case 0: temp=0;
                break;
        case 1: temp=11;
                break;
        case 2: temp=11;
                break;
        case 3: temp=11;
                break;
        case 4: temp=11;
                break;
        case 5: temp=11;
                break;
        case 6: temp=22;
                break;
        case 7: temp=33;
                break;
        case 8: temp=44;
                break;
    }

    for(y = 0; y < 11; y++)
        for (x = 0; x < (target_WIDTH/2); x++)
            PlotPixelTransparent(x+numx, y+numy, targetData[(y+temp)*(target_WIDTH/2)+x]);
}

//******************************************************************************
int nameinput(char namesent[])
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

            //writetext(20,135,"          _");

            for(y=135; y<145; y++)
                for(x=20; x<=100; x++)
                    PlotPixel(x,y,0x0000);

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

    return posz;
}

void Sleep(int i) {
    int count = i * 60 * 60;

    while(count--) {
        WaitForVblank();
    }
}
