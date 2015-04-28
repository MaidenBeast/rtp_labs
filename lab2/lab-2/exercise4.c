#include <stdio.h>
#include "delayLib.h"
#include "vxWorks.h"
#include "common.h"

int led_screen_pid;
int get_input_pid;

int pixels[16][8][3]; //3D matrix containing values between 0 and 1, meaning "turn on/off the LED"

//unsigned int rgb_leds[8][3];

/*void get_input() {
	printf("get_input\n");
	unsigned int led_no;
	unsigned int r;
	unsigned int g;
	unsigned int b;

	while (1) {
		scanf("%d %d %d %d", &led_no, &r, &g, &b);
		rgb_leds[led_no][0] = r; //red
		rgb_leds[led_no][1] = g; //green
		rgb_leds[led_no][2] = b; //blue
	}

}*/

void led_screenEx4() {
	int i;
	int j;

	sysOutByte(0x184, 0x01); //enable writing bytes to LED card
	sysOutByte(0x180, 0xFF); //turns off red LEDs
	sysOutByte(0x181, 0xFF); //turns off green LEDs
	sysOutByte(0x182, 0xFF); //turns off blue LEDs
	sysOutByte(0x183, 0x01); //enable reading trigger

	while (1) {
		//int i;
		//int j;

		if (checkTrigger()) { //if a pulse if detected from the reflex detector
			delayMsec(1);
			for (i = 0; i< 16; i++) { //for each time step on the 128us time interval (so until the 16th one)
				//default behaviour: all the LEDs turned off
				char r_led = 0xFF;
				char g_led = 0xFF;
				char b_led = 0xFF;

				for (j = 0; j < 8; j++) { //for each LED

					//red
					if (pixels[i][j][0]!=0) {	//if the RED value is still "on"
						//shift the first ONE bit to the right to j positions, i.e. is selecting the j-th LED
						//then the j-th bit of r_led byte is turned on (i.e. is set to ZERO)
						r_led &= ~(0x80 >> j);
					}

					//green
					if (pixels[i][j][1]!=0) {	//if the GREEN value is still "on"
						//shift the first ONE bit to the right to j positions, i.e. is selecting the j-th LED
						//then the j-th bit of r_led byte is turned on (i.e. is set to ZERO)
						g_led &= ~(0x80 >> j);
					}

					//blue
					if (pixels[i][j][2]!=0) {	//if the BLUE value is still "on"
						//shift the first ONE bit to the right to j positions, i.e. is selecting the j-th LED
						//then the j-th bit of r_led byte is turned on (i.e. is set to ZERO)
						b_led &= ~(0x80 >> j);
					}	

				}

				sysOutByte(0x180, r_led);
				sysOutByte(0x181, g_led);
				sysOutByte(0x182, b_led);
				delayUsec(128);		//delay of 128 microseconds
			}
			
			//turning off LEDs
			sysOutByte(0x180, 0xFF);
			sysOutByte(0x181, 0xFF);
			sysOutByte(0x182, 0xFF);
		}

	}

}

void startEx4() {
	sysClkRateSet(5000);

	int i;

	for(i = 0; i < 16; i++)
	{
		pixels[i][0][0] = 1; //all reds on the first row
	}

	for(i = 1; i < 8; i++)
	{
		pixels[0][i][1] = 1; //all green in the first column
	}	

	pixels[1][1][2] = 1; 
	pixels[3][2][2] = 1;
	pixels[5][3][2] = 1;
	pixels[7][4][2] = 1;
	pixels[9][5][2] = 1;
	pixels[11][6][2] = 1;
	pixels[11][7][2] = 1;
	pixels[12][7][2] = 1;
	pixels[13][7][2] = 1;

	led_screen_pid = taskSpawn("led_screen", 200, 0, 1000, led_screenEx4);
	//get_input_pid = taskSpawn("get_input", 200, 0, 1000, get_input);

	/*printf("get_input\n");
	unsigned int led_no;
	unsigned int r;
	unsigned int g;
	unsigned int b;

	while (1) {
		scanf("%d %d %d %d", &led_no, &r, &g, &b);
		rgb_leds[led_no][0] = r; //red
		rgb_leds[led_no][1] = g; //green
		rgb_leds[led_no][2] = b; //blue
	}*/

}

void stopEx4() {
	taskDelete(led_screen_pid);
	//taskDelete(get_input_pid);

	sysOutByte(0x180, 0xFF);
	sysOutByte(0x181, 0xFF);
	sysOutByte(0x182, 0xFF);
}
