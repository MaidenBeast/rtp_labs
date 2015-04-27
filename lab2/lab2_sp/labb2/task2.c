#include "delayLib.h"
#include "semLib.h"

int t2id;
SEM_ID semId;

int leds[8][3];

void input()
{
	char inString[20];		//input string
	char* stringToken;		//used for string splitting
	int ledNumber;			//led to change color on
	int i;					//used in for-loop
	
	while(1)
	{		
		printf("Mata in diodnr samt R/G/B: ");
		gets(&inString);
		stringToken = strtok(inString, " ");
		ledNumber = atoi(stringToken);
		printf("%d ", ledNumber);
		
		semTake(semId, WAIT_FOREVER);
		for(i = 0; i < 3; i++)
		{
			stringToken = strtok(NULL, " ");
			leds[ledNumber][i] = atoi(stringToken);
			printf("%d ", leds[ledNumber][i]);
		}
		semGive(semId);		
	}
	
}

void task2()
{	
	int i, j;		//used in for-loops
	
	while(1)
	{
		for (i = 0; i < 255; i++)
		{
			unsigned char red = 0xFF;		//out red
			unsigned char green = 0xFF;		//out green
			unsigned char blue = 0xFF;		//out blue
							
			//set bits				
			for(j = 0; j < 8; j++)
			{
				red <<= 1;
				green <<= 1;
				blue <<= 1;
				
				semTake(semId, WAIT_FOREVER);
				//set reds
				if(leds[j][0] <= i)
				{
					red = red | 0x01;
				}
				
				//set greens
				if(leds[j][1] <= i)
				{
					green = green | 0x01;
				}
				
				//set blues
				if(leds[j][2] <= i)
				{
					blue = blue | 0x01;
				}
				semGive(semId);
			}
							
			//light leds
			sysOutByte(0x180, red);
			sysOutByte(0x181, green);
			sysOutByte(0x182, blue);
			
			//sleep
			delayUsec(78);
			
		}
	}	
}

void task2Start()	//start task 2
{
	int i;
	semId = semBCreate(0, SEM_EMPTY);
	
	sysOutByte(0x184, 1);
	
	for(i = 0; i < 3; i++)
	{
		leds[0][i] = 255;
		leds[1][i] = 200;
		leds[2][i] = 150;
		leds[3][i] = 100;
		leds[4][i] = 50;
		leds[5][i] = 0;
	}
	leds[6][0] = 255;
	leds[7][1] = 255;
	
	semGive(semId);
	
	
	t2id = taskSpawn("task2", 100, 0, 4000, task2);
}

void s2()	//stop task 2
{
	taskDelete(t2id);
	
	sysOutByte(0x180, 0xff);
	sysOutByte(0x181, 0xff);
	sysOutByte(0x182, 0xff);
}
