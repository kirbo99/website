#include <FastLED.h>

#define LED_ChainLeft 5
#define LED_ChainBottom 6
#define LED_ChainRight 7
#define LED_ChainTop 8

#define NUM_LEDS 4

CRGB rightLeds[4];
CRGB topLeds[4];
CRGB leftLeds[4];
CRGB bottomLeds[4];


CRGB leds[4][4] = {rightLeds, topLeds, leftLeds, bottomLeds};


void setup() {

	Serial.begin(9600);

	FastLED.addLeds<WS2812, LED_ChainRight, GRB>(leds[0], NUM_LEDS);
	FastLED.addLeds<WS2812, LED_ChainTop, GRB>(leds[1], NUM_LEDS);
	FastLED.addLeds<WS2812, LED_ChainLeft, GRB>(leds[2], NUM_LEDS);
	FastLED.addLeds<WS2812, LED_ChainBottom, GRB>(leds[3], NUM_LEDS);


	FastLED.show();
}

void loop() {

	bool test[4] = {false, true, false, false};



	while (test[0] || test[1] || test[2]) {

		for (int p = 0; p <= NUM_LEDS; p++) {
			
		
			for (int i = 0; i < NUM_LEDS; i++) {

				for (int b = 0; b < NUM_LEDS; b++) {
					leds[i][b] = CRGB::Black;
				}

				if (!test[i]) { // Check for fire in the current index, bottom index always has no fire

					if (i == 1) { // If the current index is the Top LED path

						if (test[0] && test[2]) { // Check if fire is on the left and right side
							leds[i][p] = CRGB::Green;
							leds[i][NUM_LEDS - 1] = p%2 == 0 ? CRGB::Blue : CRGB::Black;
							
						} else if (test[0] || test[2]) { // Check if fire is on the left or right side
							leds[i][NUM_LEDS - p] = CRGB::Orange;
						}
					}

					if (i == 0 || i == 2) {
						leds[i][p] = CRGB::Green; // Set the current index to green
						leds[i][NUM_LEDS - 1] = p%2 == 0 ? CRGB::Blue : CRGB::Black;
					}


					if (i == 3) {
						if (test[0] && test[1]  && test[2]) { // If fire on all main paths
							leds[i][p] = CRGB::Green;
							leds[i][NUM_LEDS - 1] = p%2 == 0 ? CRGB::Blue : CRGB::Black;

						} else {
							leds[i][NUM_LEDS - p] = CRGB::Green;
						}
					} 

				} else { // Fire is on this path
					leds[i][NUM_LEDS - p] = CRGB::Red; 
				}

				FastLED.show();
			}

			delay(500);

		}

	}

}



