#include <stdio.h>
#include "delayLib.h"
#include "vxWorks.h"

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

void led_screenEx2() {

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

	sysOutByte(0x184, 0x01); //enable writing bytes to LED card
	sysOutByte(0x180, 0xFF); //turns off red LEDs
	sysOutByte(0x181, 0xFF); //turns off green LEDs
	sysOutByte(0x182, 0xFF); //turns off blue LEDs

	while (1) {
		int i;
		int j;

		unsigned int rgb_leds_temp[8][3]; //temporary variable

		for (i = 0; i < 8; i++) {
			for (j = 0; j<3; j++) {
				rgb_leds_temp[i][j] = rgb_leds[i][j]; //set to the temp variable the values of the rgb_leds matrix
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
			sysOutByte(0x180, r_led);	//red
			sysOutByte(0x181, g_led);	//green
			sysOutByte(0x182, b_led);	//blue

			//sysOutByte(0x180, 0x00);

			delayUsec(100);

		}
	}

}

void startEx2() {
	led_screen_pid = taskSpawn("led_screen", 200, 0, 1000, led_screenEx2);
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

void stopEx2() {
	taskDelete(led_screen_pid);
	//taskDelete(get_input_pid);

	sysOutByte(0x180, 0xFF);
	sysOutByte(0x181, 0xFF);
	sysOutByte(0x182, 0xFF);
}
