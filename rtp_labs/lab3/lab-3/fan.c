#include "common.h"
#include "fan.h"

int fan_counter = 0;

void fan() {
}

void changeFanMode(fan_mode_t mode) {
	fan_counter = 0;
	fan_mode = mode;
}
