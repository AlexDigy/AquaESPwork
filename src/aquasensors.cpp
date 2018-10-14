#include "aquasensors.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define ONE_WIRE_BUS 4
#define TEMPERATURE_PRECISION 9
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//DeviceAddress dsEpraAddr, dsAmbientAddr;
// Assign address manually. The addresses below will beed to be changed
// to valid device addresses on your bus. Device address can be retrieved
// by using either oneWire.search(deviceAddress) or individually via
// sensors.getAddress(deviceAddress, index)
DeviceAddress dsEpraAddr = { 0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF0 };
DeviceAddress dsAmbientAddr   = { 0x28, 0xFF, 0x2F, 0x6C, 0x53, 0x14, 0x01, 0x4B };
DeviceAddress dsWaterAddr   ={0x28, 0x6D, 0x7A, 0x6B, 0x05, 0x00, 0x00, 0x76};

#define DHTPIN 5      // Pin which is connected to the DHT sensor.
#define DHTTYPE DHT22 // DHT 22 (AM2302)
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

void SensorsSetup()
{
    sensors.begin();
    if (!sensors.getAddress(dsEpraAddr, 0))
        Serial.println("Unable to find address for Device 0");
    if (!sensors.getAddress(dsAmbientAddr, 1))
        Serial.println("Unable to find address for Device 1");
    // show the addresses we found on the bus
    Serial.print("Device 0 Address: ");
    printAddress(dsEpraAddr);
    Serial.println();
    Serial.print("Device 1 Address: ");
    printAddress(dsAmbientAddr);
    Serial.println();
    // set the resolution to 9 bit per device
    sensors.setResolution(dsEpraAddr, TEMPERATURE_PRECISION);
    sensors.setResolution(dsAmbientAddr, TEMPERATURE_PRECISION);

    Serial.print("Device 0 Resolution: ");
    Serial.print(sensors.getResolution(dsEpraAddr), DEC);
    Serial.println();
    Serial.print("Device 1 Resolution: ");
    Serial.print(sensors.getResolution(dsAmbientAddr), DEC);
    Serial.println();

    dht.begin();
    // Print temperature sensor details.
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.println("Temperature");
    Serial.print("Sensor:       ");
    Serial.println(sensor.name);
    Serial.print("Driver Ver:   ");
    Serial.println(sensor.version);
    Serial.print("Unique ID:    ");
    Serial.println(sensor.sensor_id);
    Serial.print("Max Value:    ");
    Serial.print(sensor.max_value);
    Serial.println(" *C");
    Serial.print("Min Value:    ");
    Serial.print(sensor.min_value);
    Serial.println(" *C");
    Serial.print("Resolution:   ");
    Serial.print(sensor.resolution);
    Serial.println(" *C");
    Serial.println("------------------------------------");
    // Print humidity sensor details.
    dht.humidity().getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.println("Humidity");
    Serial.print("Sensor:       ");
    Serial.println(sensor.name);
    Serial.print("Driver Ver:   ");
    Serial.println(sensor.version);
    Serial.print("Unique ID:    ");
    Serial.println(sensor.sensor_id);
    Serial.print("Max Value:    ");
    Serial.print(sensor.max_value);
    Serial.println("%");
    Serial.print("Min Value:    ");
    Serial.print(sensor.min_value);
    Serial.println("%");
    Serial.print("Resolution:   ");
    Serial.print(sensor.resolution);
    Serial.println("%");
    Serial.println("------------------------------------");
    // Set delay between sensor readings based on sensor details.
    delayMS = sensor.min_delay / 1000;
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // zero pad the address if necessary
        if (deviceAddress[i] < 16)
            Serial.print("0");
        Serial.print(deviceAddress[i], HEX);
    }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
    float tempC = sensors.getTempC(deviceAddress);
    Serial.print("Temp C: ");
    Serial.print(tempC);
    Serial.print(" Temp F: ");
    Serial.print(DallasTemperature::toFahrenheit(tempC));
}

// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress)
{
    Serial.print("Resolution: ");
    Serial.print(sensors.getResolution(deviceAddress));
    Serial.println();
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
    Serial.print("Device Address: ");
    printAddress(deviceAddress);
    Serial.print(" ");
    printTemperature(deviceAddress);
    Serial.println();
}

void SensorsLoop()
{
    // call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus
    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures();
    Serial.println("DONE");
    // print the device information
    printData(dsEpraAddr);
    printData(dsAmbientAddr);

    // Delay between measurements.
    //delay(delayMS);
    // Get temperature event and print its value.
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
        Serial.println("Error reading temperature!");
    }
    else
    {
        Serial.print("Temperature: ");
        Serial.print(event.temperature);
        Serial.println(" *C");
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
        Serial.println("Error reading humidity!");
    }
    else
    {
        Serial.print("Humidity: ");
        Serial.print(event.relative_humidity);
        Serial.println("%");
    }
}