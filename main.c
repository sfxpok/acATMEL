#include <reg51.h>

sbit LEDState = P1^0; // LED output
sbit buttonS1 = P1^1; // button to enable LED

bit changeIntensityState = 0;
bit clappingDetected = 0;
unsigned char timerCounter = 0;
unsigned char timerClap = 0;

bit modifiedIntensity = 0;
bit LEDEnabled = 0;

#define maxTime 100
#define zero 0
#define twentyFive 25
#define fifty 50
#define seventyFive 75
#define oneHundred 100

unsigned char LEDBrightness = oneHundred;

void initConfig() {

    EA = 1; // enable interruptions
    ET0 = 1; // enable TIMER INTERRUPTION 0
    EX0 = 1; // enable EXTERNAL INTERRUPTION 0
    EX1 = 1; // enable EXTERNAL INTERRUPTION 1
    ET1 = 1; // enable TIMER INTERRUPTION 1

    TMOD &= 0xF0; // wipe 4 bits from timer 0
    TMOD |= 0x02; // mode 2 of timer 0

    // timer 0 config
    TH0 = 0x38; // 200us
    TL0 = 0x38;

    // timer 1 config
    TH1 = 0x38; // 200us
    TL1 = 0x38;

    TR0 = 1; // start timer 0
    TR1 = 1; // start timer 1
    IT0 = 1; // external interruption enables when falling edge happens
    IT1 = 1; // external interruption enables when falling edge happens

    // LED must start disabled
    LEDState = 0;

}

// EXTERNAL INTERRUPTION 0 - changeIntensityState
void External0_ISR() interrupt 0 {
    changeIntensityState = 1;
}

// EXTERNAL INTERRUPTION 1 - clappingDetected
void External1_ISR() interrupt 2 {
    clappingDetected = 1;
}

// TIMER INTERRUPTION 0 - assigned to counting LED runtime
void Timer0_ISR() interrupt 1 {
    timerCounter++;
}

// TIMER INTERRUPTION 1
void Timer1_ISR() interrupt 3 {
    timerClap++;
}

void turnOnOffLED() {
        
    if (LEDEnabled == 0) {
        LEDState = 1;
        LEDEnabled = 1;
        LEDBrightness = oneHundred;
    } else {
        LEDState = 0;
        LEDEnabled = 0;
        LEDBrightness = zero;
    }

}

void main() {

    initConfig();

    for(;;) {

        // enable/disable LED state
        if (buttonS1 == 0) {
			
            turnOnOffLED();

            // wait until S1 is "unpressed"
            while (buttonS1 == 0) {
                // 
            };
			
        }
		
        // detecting clapping
        if (clappingDetected == 1) {
			
            turnOnOffLED();
			
            clappingDetected = 0;
			
        }
		
        if (LEDEnabled == 1) {
			
            if (timerCounter == LEDBrightness) {
                LEDState = 0;
            }
			
            if (timerCounter == maxTime) {
                timerCounter = 0;
                LEDState = 1;
            }
			
            // change LED intensity if S2 is pressed
            if (changeIntensityState == 1) {
                switch (LEDBrightness) {
                case zero:
                    LEDBrightness = twentyFive;
                    break;
                case twentyFive:
                    LEDBrightness = fifty;
                    break;
                case fifty:
                    LEDBrightness = seventyFive;
                    break;
                case seventyFive:
                    LEDBrightness = oneHundred;
                    LEDState = 1;
                    break;
                case oneHundred:
                    LEDBrightness = twentyFive;
                    break;
                }

                changeIntensityState = 0;

            }

        }
		
    }
}