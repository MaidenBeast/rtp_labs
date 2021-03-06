#include <stdio.h>
#include "delayLib.h"
#include "vxWorks.h"
#include "common.h"

int led_screen_pid;
int get_input_pid;
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

void led_screenEx3() {

	unsigned int rgb_leds[8][3] = {
			{255, 255, 255},
			{192, 192, 192},
			{128, 128, 128},
			{64, 64, 64},
			{0, 0, 0},
			{0, 255, 0},
			{0, 128, 128},
			{0, 0, 255}
	};

	int i;
	int j;

	/*for (i = 0; i < 8; i++) {
		for (j = 0; j<3; j++) {
			rgb_leds[i][j] = 0;
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
			unsigned int rgb_leds_temp[8][3];

			for (i = 0; i < 8; i++) {
				for (j = 0; j<3; j++) {
					rgb_leds_temp[i][j] = rgb_leds[i][j];
				}
			}

			for (i = 0; i < 256; i++) { //for each time step (so until the 256th one)
				//default behaviour: all the LEDs turned off
				char r_led = 0xFF;
				char g_led = 0xFF;
				char b_led = 0xFF;

				for (j = 0; j<8; j++) { //for each LED

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

				//write to the LED card
				/*sysOutByte(0x180, r_led);
				sysOutByte(0x181, g_led);
				sysOutByte(0x182, b_led);*/

				sysOutByte(0x180, 0x00); //red test

				//delayMsec(10);

			}
			delayMsec(2);
			
			//turning off LEDs
			sysOutByte(0x180, 0xFF);
			sysOutByte(0x181, 0xFF);
			sysOutByte(0x182, 0xFF);
		}

	}

}

void startEx3() {
	sysClkRateSet(1000);

	led_screen_pid = taskSpawn("led_screen", 200, 0, 1000, led_screenEx3);
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

void stopEx3() {
	taskDelete(led_screen_pid);
	//taskDelete(get_input_pid);

	sysOutByte(0x180, 0xFF);
	sysOutByte(0x181, 0xFF);
	sysOutByte(0x182, 0xFF);
}
