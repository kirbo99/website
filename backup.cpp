#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to digital port 4
#define ONE_WIRE_BUS 4
// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

DeviceAddress sensor1 = { 0x28, 0xF4, 0x5, 0x58, 0xD4, 0xE1, 0x3C, 0x8D };  // blue wire sensor's mac address
DeviceAddress sensor2 = { 0x28, 0xF3, 0xC3, 0x58, 0xD4, 0xE1, 0x3C, 0xB6 }; // red wire sensor's mac address
DeviceAddress sensor3=  { 0x28, 0x8C, 0xA2, 0x58, 0xD4, 0xE1, 0x3C, 0xD6 }; // grey wire sensor's mac address


void setup(void){      // this is important 
  Serial.begin(9600);
  sensors.begin();
}

void loop(void){ 
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  Serial.print("Sensor 1(*C): "); // displays temperature of sensor 1
  Serial.print(sensors.getTempC(sensor1)); 
  Serial.print(" Sensor 1(*F): ");
  Serial.println(sensors.getTempF(sensor1)); 
 
  Serial.print("Sensor 2(*C): "); // displays temperature of sensor 2
  Serial.print(sensors.getTempC(sensor2)); 
  Serial.print(" Sensor 2(*F): ");
  Serial.println(sensors.getTempF(sensor2)); 
  
  Serial.print("Sensor 3(*C): "); // displays temperature of sensor 3
  Serial.print(sensors.getTempC(sensor3)); 
  Serial.print(" Sensor 3(*F): ");
  Serial.println(sensors.getTempF(sensor3)); 
  
  delay(2000);
}