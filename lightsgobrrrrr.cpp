#include <FastLED.h>
#include <Thread.h>

#define LED_ChainLeft 5
#define LED_ChainBottom 6
#define LED_ChainRight 7
#define LED_ChainTop 8

#define NUM_LEDS 4

CRGB leftLeds[4];
CRGB bottomtLeds[4];
CRGB rightLeds[4];
CRGB topLeds[4];

CRGB leds[4][4] = {leftLeds, bottomtLeds, rightLeds, topLeds};

// Thread myThread;


void setup() {

	Serial.begin(9600);

	FastLED.addLeds<WS2812, LED_ChainLeft, GRB>(leds[0], NUM_LEDS);
	FastLED.addLeds<WS2812, LED_ChainBottom, GRB>(leds[1], NUM_LEDS);
	FastLED.addLeds<WS2812, LED_ChainRight, GRB>(leds[2], NUM_LEDS);
	FastLED.addLeds<WS2812, LED_ChainTop, GRB>(leds[3], NUM_LEDS);

	// fill_solid(leftLeds, NUM_LEDS, CRGB::Red);
	// fill_solid(bottomtLeds, NUM_LEDS, CRGB::Green);
	// fill_solid(rightLeds, NUM_LEDS, CRGB::Blue);
	// fill_solid(topLeds, NUM_LEDS, CRGB::Yellow);
	// myThread.enabled(true);

	FastLED.show();
}

void loop() {

	int greenIndex[NUM_LEDS] = {0}; 

	for (int i = 0; i < NUM_LEDS; i++) {

		for (int j = 0; j < NUM_LEDS-1; j++) {
			leds[i][j] = CRGB::Black;
		}
        leds[i][greenIndex[i]] = CRGB::Green; // Set the current index to green

        leds[i][NUM_LEDS - 1] = CRGB::Blue; // Set the last index to blue
        greenIndex[i] = (greenIndex[i] + 1) % NUM_LEDS; // Update the green LED index

        FastLED.show();
         // Adjust delay for desired effect
    }

	delay(500);

}



