#include "delayLib.h"
#include "semLib.h"

void drawThread();

int t3id, animid;
extern SEM_ID semId;

int pixels[16][8][3];

void task3()
{	
	int i, j, k;		//used in for-loops		
	
	sysOutByte(0x183, 0xFF); //turn on input for reflex detector
	
	while(1)
	{
		char reflexDetector = 0;
		
		//wait for reflex detector
		while((reflexDetector & 0x01) == 0)
		{
			reflexDetector = sysInByte(0x183);
		}
		
		delayUsec(500);				
		
		for(i = 0; i < 16; i++)
		{
			for(j = 0; j < 8; j++)
			{
				unsigned char red = 0xFF;		//out red
				unsigned char green = 0xFF;		//out green
				unsigned char blue = 0xFF;		//out blue
				unsigned char temp;
				
				//set bits
				for(k = 0; k < 8; k++)
				{
					red >>= 1;
					green >>= 1;
					blue >>= 1;
					
					//read pixels and set red
					semTake(semId, WAIT_FOREVER);
					temp = pixels[i][k][0];
					semGive(semId);
					
					temp >>= 5;
					if(temp <= j)
					{
						red = red | 0x80;
					}
					
					//read pixels and set green
					semTake(semId, WAIT_FOREVER);
					temp = pixels[i][k][1];
					semGive(semId);
					
					temp >>= 5;
					if(temp == 0)
					{
						green = green | 0x80;
					}
					
					//read pixels and set blue
					semTake(semId, WAIT_FOREVER);
					temp = pixels[i][k][2];
					semGive(semId);
					
					temp >>= 5;
					if(temp == 0)
					{
						blue = blue | 0x80;
					}
					
					
				}
				
				//light leds
				sysOutByte(0x180, red);
				sysOutByte(0x181, green);
				sysOutByte(0x182, blue);
				
				delayUsec(25);
			}
			
		}
		
		//turn off leds
		sysOutByte(0x180, 0xFF);
		sysOutByte(0x181, 0xFF);
		sysOutByte(0x182, 0xFF);
		
		//sleep
		taskDelay(50);					
	}	
}

void task3Start()
{		
	int i;
	semId = semBCreate(0, SEM_FULL);
	sysClkRateSet(1000);
	sysOutByte(0x184, 1);
	
	for(i = 0; i < 16; i++)
	{
		pixels[i][0][0] = 15*i;
	}
	
	for(i = 1; i < 8; i++)
	{
		pixels[0][i][1] = 255;
	}	
	
	pixels[1][1][2] = 255;
	pixels[3][2][2] = 255;
	pixels[5][3][2] = 255;
	pixels[7][4][2] = 255;
	pixels[9][5][2] = 40;
	pixels[11][6][2] = 40;
	pixels[11][7][2] = 255;
	pixels[12][7][2] = 100;
	pixels[13][7][2] = 40;
	
	
	t3id = taskSpawn("task3", 100, 0, 4000, task3);
}

void s3()
{
	taskDelete(t3id);
	
	sysOutByte(0x180, 0xff);
	sysOutByte(0x181, 0xff);
	sysOutByte(0x182, 0xff);
}

void animStart()
{
	sysClkRateSet(1000);
	sysOutByte(0x184, 1);
	semId = semBCreate(0, SEM_FULL);
	animid = taskSpawn("anim", 100, 0, 4000, drawThread);
	t3id = taskSpawn("task3", 101, 0, 4000, task3);
}
