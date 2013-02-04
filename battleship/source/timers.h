#ifndef TIMERS_H
#define TIMERS_H


#define TIME_FREQUENCY_SYSTEM 0x0
#define TIME_FREQUENCY_64 0x1
#define TIME_FREQUENCY_256 0x2
#define TIME_FREQUENCY_1024 0x3
#define TIME_OVERFLOW 0x4
#define TIME_ENABLE 0x80
#define TIME_IRQ_ENABLE 0x40

#endif

//All right and here is our simple function to wait a few seconds

void Wait(int seconds)
{
//Start the timer
REG_TM3CNT = FREQUENCY_1024 | TIMER_ENABLE;
//zero the timer
REG_TM3D = 0;
while(seconds--)
{
while(REG_TM3D <= 16386){} //wait
REG_TM3D = 0; //reset the timmer
}
}

