#include <stdio.h>
#include "delayLib.h"
#include "vxWorks.h"
#include "common.h"

int led_screen_pid;
int get_input_pid;

int pixels[16][8][3];

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

	for(i = 0; i < 16; i++)
	{
		pixels[i][0][0] = 255;
	}

	for(i = 1; i < 8; i++)
	{
		pixels[0][i][1] = 255;
	}	

	pixels[1][1][2] = 255;
	pixels[3][2][2] = 255;
	pixels[5][3][2] = 255;
	pixels[7][4][2] = 255;
	pixels[9][5][2] = 255;
	pixels[11][6][2] = 255;
	pixels[11][7][2] = 255;
	pixels[12][7][2] = 255;
	pixels[13][7][2] = 255;
	
	int enableLEDs = 0;

	sysOutByte(0x184, 0x01); //enable writing bytes to LED card
	sysOutByte(0x180, 0xFF); //turns off red LEDs
	sysOutByte(0x181, 0xFF); //turns off green LEDs
	sysOutByte(0x182, 0xFF); //turns off blue LEDs
	sysOutByte(0x183, 0x01); //enable reading trigger

	while (1) {
		//int i;
		//int j;

		if (checkTrigger()) {
			enableLEDs = 1;
			delayMsec(1);
		}

		if (enableLEDs) {
			for (i = 0; i< 16; i++) {

				unsigned int rgb_leds_temp[8][3];

				for (j = 0; j < 8; j++) {
					for (k = 0; k<3; k++) {
						rgb_leds_temp[j][k] = pixels[i][j][k] >> 5;
					}
				}

				for (j = 0; j < 8; j++) {

					char r_led = 0xFF;
					char g_led = 0xFF;
					char b_led = 0xFF;

					for (k = 0; k < 8; k++) {
						/*char r_temp = (char)(pixels[i][k][0] >> 5)-j;
						char g_temp = (char)(pixels[i][k][1] >> 5)-j;
						char b_temp = (char)(pixels[i][k][2] >> 5)-j;*/

						/*//red
						if (r_temp <= j) {
							r_led &= ~(0x80 >> k);
						}

						//green
						if (g_temp <= j) {
							g_led &= ~(0x80 >> k);
						}

						//blue
						if (b_temp <= j) {
							b_led &= ~(0x80 >> k);
						}*/

						//red
						if (rgb_leds_temp[k][0]>0) {
							r_led &= ~(0x80 >> k);
							rgb_leds_temp[k][0]--;
						}

						//green
						if (rgb_leds_temp[k][1]>0) {
							g_led &= ~(0x80 >> k);
							rgb_leds_temp[k][1]--;
						}

						//blue
						if (rgb_leds_temp[k][2]>0) {
							b_led &= ~(0x80 >> k);
							rgb_leds_temp[k][2]--;
						}
					}

					sysOutByte(0x180, r_led);
					sysOutByte(0x181, g_led);
					sysOutByte(0x182, b_led);

					delayUsec(128);

				}
			}
		}

		sysOutByte(0x180, 0xFF);
		sysOutByte(0x181, 0xFF);
		sysOutByte(0x182, 0xFF);
		enableLEDs = 0;
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

	sysOutByte(0x180, 0xFF);
	sysOutByte(0x181, 0xFF);
	sysOutByte(0x182, 0xFF);
}
