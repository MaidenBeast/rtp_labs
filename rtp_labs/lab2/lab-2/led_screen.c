#include <stdio.h>
#include "delayLib.h"
#include "vxWorks.h"

int led_screen_pid;

void led_screen() {
	//unsigned int rgb_leds[8][3];

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

		unsigned int rgb_leds_temp[8][3];

		for (i = 0; i < 8; i++) {
			for (j = 0; j<3; j++) {
				rgb_leds_temp[i][j] = rgb_leds[i][j];
			}
		}

		for (i = 0; i < 256; i++) {
			char r_led = 0xFF;
			char g_led = 0xFF;
			char b_led = 0xFF;

			for (j = 0; j<8; j++) {
				
				//red
				if (rgb_leds_temp[j][0]>0) {
					r_led &= ~(0x80 >> j);
					rgb_leds_temp[j][0]--;
				}

				//green
				if (rgb_leds_temp[j][1]>0) {
					g_led &= ~(0x80 >> j);
					rgb_leds_temp[j][1]--;
				}

				//blue
				if (rgb_leds_temp[j][2]>0) {
					b_led &= ~(0x80 >> j);
					rgb_leds_temp[j][2]--;
				}
				
			}

			sysOutByte(0x180, r_led);
			sysOutByte(0x181, g_led);
			sysOutByte(0x182, b_led);

			//delayUsec(0.1/256);
		}
	}

}

void start() {
	led_screen_pid = taskSpawn("led_screen", 200, 0, 1000, led_screen);
}

void stop() {
	taskDelete(led_screen_pid);
}
