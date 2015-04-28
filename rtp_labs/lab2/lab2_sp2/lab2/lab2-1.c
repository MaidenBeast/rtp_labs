#include <stdio.h>
#include <math.h>
#include <vxWorks.h>
#include "vxWorks.h"
#include "tickLib.h"
#include "sysLib.h"
#include "delayLib.h"
#include "semLib.h" 
#include "lab3.c"


#define red		0x180
#define green	0x181
#define blue	0x182


int run = 1;

int pixels[16][8][3];/*={
		{{255,255,255}, {226,226,226}, {180,180,180}, {144,144,144}, {108,108,108}, {72,72,72}, {36,36,36}, {255,0,255}},
		{{255,255,255}, {226,226,226}, {180,180,180}, {144,144,144}, {108,108,108}, {72,72,72}, {36,36,36}, {255,0,255}},
		{{255,255,255}, {226,226,226}, {180,180,180}, {144,144,144}, {108,108,108}, {72,72,72}, {36,36,36}, {255,0,255}},
		{{255,255,255}, {226,226,226}, {180,180,180}, {144,144,144}, {108,108,108}, {72,72,72}, {36,36,36}, {255,0,255}},
		{{255,255,255}, {226,226,226}, {180,180,180}, {144,144,144}, {108,108,108}, {72,72,72}, {36,36,36}, {255,0,255}},
		{{255,255,255}, {226,226,226}, {180,180,180}, {144,144,144}, {108,108,108}, {72,72,72}, {36,36,36}, {255,0,255}},
		{{0,0,0}, {72,72,72}, {36,36,36}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}},
		{{255,255,255}, {226,226,226}, {180,180,180}, {144,144,144}, {108,108,108}, {72,72,72}, {36,36,36}, {255,0,255}},
		{{255,255,255}, {226,226,226}, {180,180,180}, {144,144,144}, {108,108,108}, {72,72,72}, {36,36,36}, {255,0,255}},
		{{255,255,255}, {226,226,226}, {180,180,180}, {144,144,144}, {108,108,108}, {72,72,72}, {36,36,36}, {255,0,255}},
		{{255,255,255}, {226,226,226}, {180,180,180}, {144,144,144}, {108,108,108}, {72,72,72}, {36,36,36}, {255,0,255}},
		{{255,255,255}, {226,226,226}, {180,180,180}, {144,144,144}, {108,108,108}, {72,72,72}, {36,36,36}, {255,0,255}},
		{{255,255,255}, {226,226,226}, {180,180,180}, {144,144,144}, {108,108,108}, {72,72,72}, {36,36,36}, {255,0,255}},
		{{255,255,255}, {226,226,226}, {180,180,180}, {144,144,0}, {108,108,0}, {72,72,0}, {36,36,0}, {255,0,255}},
		{{255,255,255}, {226,226,226}, {180,180,180}, {144,144,0}, {108,108,0}, {72,72,0}, {36,36,0}, {255,0,255}},
		{{255,255,255}, {226,226,226}, {180,180,180}, {144,144,144}, {108,108,108}, {72,72,72}, {36,36,36}, {255,0,255}}
};*/


int delay=80;


void uppg1()
{
	sysOutByte(0x184, 0x01);
	
	//	RGBY
	sysOutByte(0x180, ~9);
	sysOutByte(0x181, ~10);
	sysOutByte(0x182, ~4);
	
}


void Uppgift2() 
{ 
	int redlamps, greenlamps, bluelamps;
	int rgbArray[8][3]={{255,0,255}, {226,1,226}, {180,50,180}, {144,144,144}, {0,0,0}, {72,72,72}, {36,36,36}, {0,0,0}};
	
	redlamps = greenlamps = bluelamps = 0;
	sysOutByte(0x184, 0x01);
	while(run == 1) 
	{ 
		redlamps = greenlamps = bluelamps = 0;
		int i; 
		for(i = 0; i < 255; i++) 
		{ 
			int j; 
			for(j = 0; j < 8; j++) 
			{ 				
				if(rgbArray[j][0] > i)
				{
					//redlamps |= redlamps << 1;
					redlamps = redlamps | 1<<j;
				}
				if(rgbArray[j][1] > i)
				{
					greenlamps = greenlamps | 1<<j;
				}
				if(rgbArray[j][2] > i)
				{
					bluelamps = bluelamps | 1<<j;
				}
			} 
			
			//delayUsec(1000000);
		}
		sysOutByte(red, ~redlamps); 
		sysOutByte(green, ~greenlamps); 
		sysOutByte(blue, ~bluelamps);
		//delayUsec(100000);
	} 
}



void Uppgift2_2()
{
	int redlamps, greenlamps, bluelamps;
	int rgbArray[8][4]={{0,255,255,255}, {1,192,192,192}, {2,128,128,128}, {3,64,64,64}, {4,0,0,0}, {5,0,255,0}, {6,0,128,128}, {7,0,0,255}};
	
	redlamps = greenlamps = bluelamps = 0;
	sysOutByte(0x184, 0x01);
	while(run == 1) 
	{ 
		redlamps = greenlamps = bluelamps = 0;
		int i; 
		for(i = 0; i < 255; i++) 
		{ 
			int j; 
			for(j = 0; j < 8; j++) 
			{ 
				
				if(rgbArray[j][1] > i)
				{
					//redlamps |= redlamps << 1;
					redlamps = redlamps | 1<<j;
				}
				if(rgbArray[j][2] > i)
				{
					greenlamps = greenlamps | 1<<j;
				}
				if(rgbArray[j][3] > i)
				{
					bluelamps = bluelamps | 1<<j;
				}
			} 
			sysOutByte(red, ~redlamps); 
			sysOutByte(green, ~greenlamps); 
			sysOutByte(blue, ~bluelamps);
		}
	}
}

void Uppgift3()
{
	int redlamps, greenlamps, bluelamps;
	//int rgbArray[8][4]={{0,255,255,255}, {1,192,192,192}, {2,128,128,128}, {3,64,64,64}, {4,0,0,0}, {5,0,255,0}, {6,0,128,128}, {7,0,0,255}};
	int sensor=1;
	redlamps = greenlamps = bluelamps = 0;
	sysOutByte(0x184, 0x01);
	sysOutByte(0x183,0x01);
	while(run == 1) 
	{ 
		sensor = sysInByte(0x183);
		if(sensor==0)
		{
			delayUsec(1000);
			sysOutByte(red, ~255); 
			sysOutByte(green, ~255); 
			sysOutByte(blue, ~255);
			delayUsec(2000);
		}
		sysOutByte(red, ~0); 
		sysOutByte(green, ~0); 
		sysOutByte(blue, ~0);
	}
}

void Uppgift4()
{
	int redlamps, greenlamps, bluelamps;
	int sensor=1;
	redlamps = greenlamps = bluelamps = 0;
	sysOutByte(0x184, 0x01);
	sysOutByte(0x183,0x01);
	while(run == 1) 
	{ 
		sensor = sysInByte(0x183);
		if(sensor==0)
		{
			delayUsec(1000);
			int k;
			
			for(k = 0; k < 16; k++){
				int i;
				redlamps = greenlamps = bluelamps = 0;
				for(i = 0; i < 255; i++) 
				{ 
					int j; 
					for(j = 0; j < 8; j++) 
					{ 
						
						if(pixels[k][j][0] !=0)
						{
							//redlamps |= redlamps << 1;
							redlamps = redlamps | 1<<j;
						}
						if(pixels[k][j][1] !=0)
						{
							greenlamps = greenlamps | 1<<j;
						}
						if(pixels[k][j][2] !=0)
						{
							bluelamps = bluelamps | 1<<j;
						}						
					} 
				}
				sysOutByte(red, ~redlamps);
				sysOutByte(green, ~greenlamps);
				sysOutByte(blue, ~bluelamps);
				delayUsec(128);
			}
		}
		sysOutByte(red, ~0); 
		sysOutByte(green, ~0);
		sysOutByte(blue, ~0);
		
	}
}
void Uppgift5()
{
	int redlamps, greenlamps, bluelamps;
	int sensor=1;
	float reddelay=0,greendelay=0,bluedelay=0,tempsum=0;
	redlamps = greenlamps = bluelamps = 0;
	sysOutByte(0x184, 0x01);
	sysOutByte(0x183,0x01);
	while(run == 1) 
	{ 
		sensor = sysInByte(0x183);
		if(sensor==0)
		{
			delayUsec(1000);
			int k;
			
			for(k = 0; k < 16; k++){
				
				int i;
				redlamps = greenlamps = bluelamps = 0;
				for(i = 0; i < 255; i++)
				{ 
					int j; 
					for(j = 0; j < 8; j++) 
					{ 
						
						if(pixels[k][j][0] > i)
						{
							//redlamps |= redlamps << 1;
							redlamps = redlamps | 1<<j;
						}
						if(pixels[k][j][1] > i)
						{
							greenlamps = greenlamps | 1<<j;
						}
						if(pixels[k][j][2] > i)
						{
							bluelamps = bluelamps | 1<<j;
						}						
					} 
				}
				// delay on/off, share delay time between rgb based upon intensity
				sysOutByte(red, ~redlamps);
				sysOutByte(green, ~greenlamps);
				sysOutByte(blue, ~bluelamps);
				delayUsec(delay);
			}
		}
		sysOutByte(red, ~0); 
		sysOutByte(green, ~0); 
		sysOutByte(blue, ~0);
		
	}
}

void stop()
{
	run = 0;
}

void start()
{
	sysOutByte(0x180,~0);
	sysOutByte(0x181,~0);
	sysOutByte(0x182,~0);
	
	run=1;
	//taskSpawn("uppgift2", 101, 0, 4000, Uppgift2);
	//taskSpawn("uppgift2_2", 101, 0, 4000, Uppgift2_2);
	//taskSpawn("uppgift3", 101, 0, 4000, Uppgift3);
	//taskSpawn("uppgift4", 101, 0, 4000, Uppgift4);
	taskSpawn("uppgift5", 101, 0, 4000, Uppgift5);
}
