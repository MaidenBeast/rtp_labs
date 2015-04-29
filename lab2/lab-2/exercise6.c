#include <stdio.h>
#include "delayLib.h"
#include "vxWorks.h"
#include "common.h"

#define RGB_LEVELS 8

void drawThread();

int led_screen_pid;
int get_input_pid;
int draw_thread_pid;

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

void led_screenEx6() {
	int i;
	int j;
	int k;

	sysOutByte(0x184, 0x01); //enable writing bytes to LED card
	sysOutByte(0x180, 0xFF); //turns off red LEDs
	sysOutByte(0x181, 0xFF); //turns off green LEDs
	sysOutByte(0x182, 0xFF); //turns off blue LEDs
	sysOutByte(0x183, 0x01); //enable reading trigger

	while (1) {
		//int i;
		//int j;

		if (checkTrigger()) {
			delayMsec(1);
			for (i = 0; i< 16; i++) {

				unsigned int rgb_leds_temp[8][3];
				
				//default behaviour: all the LEDs turned off
				char r_led = 0xFF;
				char g_led = 0xFF;
				char b_led = 0xFF;

				for (j = 0; j < 8; j++) {
					for (k = 0; k<3; k++) {
						rgb_leds_temp[j][k] = pixels[15-i][8-j][k] >> (int)log2(256/RGB_LEVELS);
					}
				}

				for (j = 0; j < 8; j++) {	//for each RGB level (0-31, 32-64,..., 224-255)

					for (k = 0; k < 8; k++) {	//for each row
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

					delayUsec(128/RGB_LEVELS); //delay for 128us/number of levels

				}
			}
			sysOutByte(0x180, 0xFF);
			sysOutByte(0x181, 0xFF);
			sysOutByte(0x182, 0xFF);
		}
	}

}

void startEx6() {
	sysClkRateSet(1000);

	draw_thread_pid = taskSpawn("draw_thread", 200, 0, 4000, drawThread);
	led_screen_pid = taskSpawn("led_screen", 201, 0, 1000, led_screenEx6);
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

void stopEx6() {
	taskDelete(led_screen_pid);
	//taskDelete(get_input_pid);
	taskDelete(draw_thread_pid);

	sysOutByte(0x180, 0xFF);
	sysOutByte(0x181, 0xFF);
	sysOutByte(0x182, 0xFF);
}
