#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to digital port 4
#define ONE_WIRE_BUS 4
// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

DeviceAddress sensor1 = { 0x28, 0xF4, 0x5, 0x58, 0xD4, 0xE1, 0x3C, 0x8D };  // blue wire the sensors mac address
DeviceAddress sensor2 = { 0x28, 0xF3, 0xC3, 0x58, 0xD4, 0xE1, 0x3C, 0xB6 }; // red wire the sensors mac address
DeviceAddress sensor3 = { 0x28, 0x8C, 0xA2, 0x58, 0xD4, 0xE1, 0x3C, 0xD6 }; // grey wire the sensors mac address

float avgTemp = 0;


void setup(void){      // this is important 
  Serial.begin(9600);
  sensors.begin();

  for (int i = 0; i < 5; i++) {
	avgTemp = avgTemp + sensors.getTempF(sensor1);
	avgTemp = avgTemp + sensors.getTempF(sensor2);
	avgTemp = avgTemp + sensors.getTempF(sensor3);
	delay(1000);
  }
  avgTemp = avgTemp / 15;
  Serial.print("Average Temp: ");
  Serial.print(avgTemp);
  Serial.println("");

}

void loop(void) { 

	sensors.requestTemperatures();

	float cTemp[3] = {sensors.getTempC(sensor1), sensors.getTempC(sensor2), sensors.getTempC(sensor3)};
	float fTemp[3] = {sensors.getTempF(sensor1), sensors.getTempF(sensor2), sensors.getTempF(sensor3)};

	Serial.print("Average Temp: ");
	Serial.print(avgTemp);
	Serial.println("");

	for (int i = 0; i<3; i++) {
		if (fTemp[i] >= (avgTemp + 10)) {
			Serial.print("Temp ");
			Serial.print(i);
			Serial.print(" has been tripped - ");
			Serial.print(fTemp[i]);
			Serial.print("F");
			Serial.println("");
		}
	}
	Serial.println("");

  delay(2000);
}