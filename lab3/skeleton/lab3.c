#include "common.h"

void lab3() {
	int input = -1
	running_mode = CONFIG;
	
	while (1) {
	
		switch (running_mode) {
			case CONFIG:
				break;
			case ERROR:
				break;
			default: 	//RUN
				break;
		}
		
		
		do { //reads from keyboard matrix until the user digit something on the keyboard
			input = readKeyboard();		
		} while (input != -1);
		
	}
}