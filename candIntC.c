#include <reg51.h>

#define intensityLevel 0
//#define powerButton 0

sbit outputLED = P1^0 // LED output
sbit buttonS1 = P1^1 // button to enable LED
sbit buttonS2 = P3^2 // button to change intensity of LED
sbit clapDetect = P3^3 // clapper detector

void initConfig() {

	EA = 1; // enable interruptions
	EX0 = 1; // enable EXTERNAL INTERRUPTION 0
	EX1 = 1; // enable EXTERNAL INTERRUPTION 1
	
}

void buttonS1Routine() {

	if (outputLED == 1) {
		outputLED = 0;
	} else {
		outputLED = 1;
	}
	
}

void buttonS2Routine() {

	if (intensityLevel != 100) {
		intensityLevel = intensityLevel + 25;
	} else {
		intensityLevel = 0;
	}

}

void main() {

	initConfig();
	
	for(;;) {
	
		if (buttonS1 == 1) {
			buttonS1Routine();
		}
		
		if (buttonS2 == 1) {
			buttonS2Routine();
		}
		
	}
	
}