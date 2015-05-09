#include "common.h"
#include "fan.h"

int fan_counter = 0;

void fan() {
}

void changeFanMode(fan_mode_t mode) {
	fan_counter = 0;
	fan_mode = mode;
}

void start_test_fan() {
	sysOutByte(0x184, 0xFF);
	fan_PID = taskSpawn("fan", 200, 0, 1000, fan);
}

void stop_test_fan() {
	taskDelete(fan_PID);
}
