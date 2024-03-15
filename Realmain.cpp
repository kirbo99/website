#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoThread.h>
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
CRGB bottomtLeds[4];
CRGB rightLeds[4];
CRGB topLeds[4];

CRGB leds[4][4] = {leftLeds, bottomtLeds, rightLeds, topLeds};


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

void setup(void) {  

	Serial.begin(9600);
	sensors.begin();

	averageTemp = getAvgTemp();
	
	bool isFire[3] = {false, false, false};
	bool fire = false;


}

void loop(void) { 

	
	sensorCheck()

	if (fire) {
		while(fire) {
			sensorCheck()
		}
	}


}



void getAvgTemp() {

	float avgTemp = 0;

	for (int i = 0; i < 5; i++) {
		avgTemp = avgTemp + sensors.getTempF(sensor1);
		avgTemp = avgTemp + sensors.getTempF(sensor2);
		avgTemp = avgTemp + sensors.getTempF(sensor3);
		delay(1000);
	}

	return avgTemp / 15;
}



void sensorCheck() {

	sensors.requestTemperatures();

	float cTemp[3] = {sensors.getTempC(sensor1), sensors.getTempC(sensor2), sensors.getTempC(sensor3)};
	float fTemp[3] = {sensors.getTempF(sensor1), sensors.getTempF(sensor2), sensors.getTempF(sensor3)};


	for (int i = 0; i < 3; i++) {
	
		
		if (fTemp[i] >= (avgTemp + 10)) { // IsFire
			isFire[i] = true;
		}
	}

	fireCheck()

}

void fireCheck() {

	for (int i = 0; i < 3; i++) {
		if (isFire[i]) { // If there is a fire, set fire to true
			fire = true;
			break;
		}
	}

	fire = false;
}

void changeLights (int i, int r, int g, int b){
	leds[i] = CRGB(r,g,b);
	FastLED.show();
}