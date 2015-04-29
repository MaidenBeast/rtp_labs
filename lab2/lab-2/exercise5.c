#include <stdio.h>
#include <math.h>
#include "delayLib.h"
#include "vxWorks.h"
#include "common.h"

#define RGB_LEVELS 8

int led_screen_pid;
int get_input_pid;

int pixels[16][8][3]; //3D matrix containing the RGB values of the "emulated" LED screen

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

void led_screenEx5() {
	int i;
	int j;
	int k;

	for(i = 0; i < 16; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 3; k++) {
				pixels[i][j][k] = 0;
			}
		}
	}

	/*for(i = 0; i < 16; i++)
	{
		pixels[i][0][0] = 255; //all reds on the first row
		pixels[i][3][1] = 255; //all green on the 4th row
		pixels[i][7][0] = 255; //all reds on the last row
	}

	for(i = 0; i < 8; i++)
	{
		pixels[0][i][0] = 255; 	//all reds on the first column
		pixels[7][i][2] = 255; 	//all blue on the 8th column
		pixels[15][i][0] = 255; 	//all reds on the last column 
	}*/

	for(i = 0; i < 8; i++) { //The Italian flag :)	
		for (j = 0; j < 16; j++) {
			if (j < 5) { //green
				pixels[j][i][1] = 255;
			} else if (j < 10) { //white
				pixels[j][i][0] = 255;
				pixels[j][i][1] = 255;
				pixels[j][i][2] = 255;
			} else if (j < 15) { //red
				pixels[j][i][0] = 255;
			}
		}
	}

	/*for(i = 0; i < 8; i++) { //The Swedish flag :)
		for (j = 0; j < 16; j++) {
			if ((j>2 && j<6) || (i>2 && i<5)) {
				pixels[j][i][0] = 255;
				pixels[j][i][1] = 255;
			} else  {
				pixels[j][i][2] = 255;
			}
		}
	}*/

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
			for (i = 0; i< 16; i++) {	//for each column

				unsigned int rgb_leds_temp[8][3];

				//default behaviour: all the LEDs turned off
				char r_led = 0xFF;
				char g_led = 0xFF;
				char b_led = 0xFF;

				for (j = 0; j < 8; j++) {
					for (k = 0; k<3; k++) {
						rgb_leds_temp[j][k] = pixels[15-i][j][k] >> (int)log2(256/RGB_LEVELS); //quantize the RGB values to RGB_LEVELS
					}
				}

				for (j = 0; j < 8; j++) {	//for each RGB level (0-31, 32-64,..., 224-255)

					for (k = 0; k < 8; k++) {	//for each row

						//red
						if (rgb_leds_temp[j][0]>0) { 	//if the RED value is still "on"
							//shift the first ONE bit to the right to j positions, i.e. is selecting the j-th LED
							//then the j-th bit of r_led byte is turned on (i.e. is set to ZERO)
							r_led &= ~(0x80 >> j);		
							rgb_leds_temp[j][0]--;		//decrement the RED value
						}

						//green
						if (rgb_leds_temp[j][1]>0) {	//if the GREEN value is still "on"
							//shift the first ONE bit to the right to j positions, i.e. is selecting the j-th LED
							//then the j-th bit of r_led byte is turned on (i.e. is set to ZERO)
							g_led &= ~(0x80 >> j);
							rgb_leds_temp[j][1]--;		//decrement the GREEN value
						}

						//blue
						if (rgb_leds_temp[j][2]>0) {	//if the BLUE value is still "on"
							//shift the first ONE bit to the right to j positions, i.e. is selecting the j-th LED
							//then the j-th bit of r_led byte is turned on (i.e. is set to ZERO)
							b_led &= ~(0x80 >> j);
							rgb_leds_temp[j][2]--;		//decrement the BLUE value
						}
					}
					sysOutByte(0x180, r_led);
					sysOutByte(0x181, g_led);
					sysOutByte(0x182, b_led);

					delayUsec(128/RGB_LEVELS); //delay for 128us/number of levels

				}
			}
			sysOutByte(0x180, 0xFF);
			sysOutByte(0x181, 0xFF);
			sysOutByte(0x182, 0xFF);
		}
	}

}

void startEx5() {
	sysClkRateSet(1000);

	led_screen_pid = taskSpawn("led_screen", 200, 0, 1000, led_screenEx5);
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

void stopEx5() {
	taskDelete(led_screen_pid);
	//taskDelete(get_input_pid);
	int i;
	int j;
	int k;

	for(i = 0; i < 16; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 3; k++) {
				pixels[i][j][k] = 0;
			}
		}
	}

	sysOutByte(0x180, 0xFF);
	sysOutByte(0x181, 0xFF);
	sysOutByte(0x182, 0xFF);
}
