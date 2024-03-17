#include <OneWire.h>
#include <DallasTemperature.h>
#include <FastLED.h>

// Data wire is connected to digital port 4
#define ONE_WIRE_BUS 3

// LED Data Pins
#define LED_ChainLeft 5
#define LED_ChainBottom 6
#define LED_ChainRight 7
#define LED_ChainTop 8

// How many LEDS are in a chain
#define NUM_LEDS 4

//Define LEDS
CRGB leftLeds[4];
CRGB bottomLeds[4];
CRGB rightLeds[4];
CRGB topLeds[4];

CRGB leds[4][4] = {rightLeds, topLeds, leftLeds, bottomLeds};


// Processing Thread
// Thread myThread;

// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

DeviceAddress sensor1 = { 0x28, 0xF4, 0x5, 0x58, 0xD4, 0xE1, 0x3C, 0x8D };  // blue wire the sensor1 mac address
DeviceAddress sensor2 = { 0x28, 0xF3, 0xC3, 0x58, 0xD4, 0xE1, 0x3C, 0xB6 }; // red wire the sensor2 mac address
DeviceAddress sensor3 = { 0x28, 0x8C, 0xA2, 0x58, 0xD4, 0xE1, 0x3C, 0xD6 }; // grey wire the sensor3 mac address

float avgTemp = 0;
bool isFire[4] = {false, false, false, false};

void setup(void) {  

	Serial.begin(9600);

	sensors.begin();
	getAvgTemp();

	FastLED.addLeds<WS2812, LED_ChainRight, GRB>(leds[0], NUM_LEDS);
	FastLED.addLeds<WS2812, LED_ChainTop, GRB>(leds[1], NUM_LEDS);
	FastLED.addLeds<WS2812, LED_ChainLeft, GRB>(leds[2], NUM_LEDS);
	FastLED.addLeds<WS2812, LED_ChainBottom, GRB>(leds[3], NUM_LEDS);

}

void loop(void) { 


	for (int i = 0; i < NUM_LEDS; i++) {
		fill_solid(leds[i], NUM_LEDS, CRGB::Yellow);
	}
	FastLED.show();

	
	sensorCheck();
	
	
	
	
	while (isFire[0] || isFire[1] || isFire[2]) {


		sensorCheck();

		for (int p = 0; p <= NUM_LEDS; p++) {
			
		
			for (int i = 0; i < NUM_LEDS; i++) {

				for (int b = 0; b < NUM_LEDS; b++) {
					leds[i][b] = CRGB::Black;
				}

				if (!isFire[i]) { // Check for fire in the current index, bottom index always has no fire

					if (i == 1) { // If the current index is the Top LED path

						if (isFire[0] && isFire[2]) { // Check if fire is on the left and right side

							leds[i][p] = CRGB::Green;
							leds[i][NUM_LEDS - 1] = p%2 == 0 ? CRGB::Blue : CRGB::Black;
							
						} else if (isFire[0] || isFire[2]) { // Check if fire is on the left or right side
							leds[i][NUM_LEDS - p] = CRGB::Orange;
						}
					}

					if (i == 0 || i == 2) {

						leds[i][p] = CRGB::Green; // Set the current index to green
						leds[i][NUM_LEDS - 1] = p%2 == 0 ? CRGB::Blue : CRGB::Black;

					}


					if (i == 3) {
						if (isFire[0] && isFire[1]  && isFire[2]) { // If fire on all main paths

							leds[i][p] = CRGB::Green;
							leds[i][NUM_LEDS - 1] = p%2 == 0 ? CRGB::Blue : CRGB::Black;

						} else {
							leds[i][NUM_LEDS - p] = CRGB::Green;
						}
					} 

				} else { // Fire is on this path, set to red
					leds[i][NUM_LEDS - p] = CRGB::Red; 
				}

				FastLED.show();
			}

			delay(500);

		}

	
	}


}



void getAvgTemp() {

	sensors.requestTemperatures();

	for (int i = 0; i < 5; i++) {
		avgTemp = avgTemp + sensors.getTempF(sensor1);
		avgTemp = avgTemp + sensors.getTempF(sensor2);
		avgTemp = avgTemp + sensors.getTempF(sensor3);
		delay(1000);
	}

	avgTemp = avgTemp / 15;
}



void sensorCheck() {

	sensors.requestTemperatures();

	// float cTemp[3] = {sensors.getTempC(sensor1), sensors.getTempC(sensor2), sensors.getTempC(sensor3)};
	float fTemp[3] = {sensors.getTempF(sensor1), sensors.getTempF(sensor2), sensors.getTempF(sensor3)};

	for (int i = 0; i < 3; i++) {
		
		if (fTemp[i] >= (avgTemp + 10)) { // IsFire
			isFire[i] = true;
		} else {
			isFire[i] = false;
		}
	}

	

}

void debugger() {

	sensors.requestTemperatures();
	float fTemp[3] = {sensors.getTempF(sensor1), sensors.getTempF(sensor2), sensors.getTempF(sensor3)};


	Serial.print("Avg Temp: ");
	Serial.print(avgTemp);
	Serial.println("");

	for (int i = 0; i < 3; i++) {
		Serial.print("Temps: ");
		Serial.print(fTemp[i]);
		Serial.print(" ");
		if (i < 2) {
			Serial.print(", ");
		}
	}
	Serial.println("");

	for (int i = 0; i < sizeof(isFire); i++) { 
		if (i == 0) {
			Serial.print("Values: ");
		}
		
		Serial.print(isFire[i]);
		if (i < sizeof(isFire) - 1) {
			Serial.print(", ");
		}
	}

	Serial.println();
	Serial.println();



	
	
}