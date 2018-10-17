#include <Arduino.h>
#include "myds18b20.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 5
#define TEMPERATURE_PRECISION 9
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress dsEpraAddr = {0x28, 0xff, 0x2f, 0x6c, 0x53, 0x14, 0x01, 0x4b};
DeviceAddress dsAmbientAddr = {0x28, 0xff, 0xd8, 0x9b, 0x53, 0x14, 0x01, 0x4b};
DeviceAddress dsWaterAddr = {0x28, 0x6D, 0x7A, 0x6B, 0x05, 0x00, 0x00, 0x76};

float dsEpraTemp = -127;
float dsAmbientTemp = -127;
float dsWaterTemp = -127;

void DallasBegin()
{
    sensors.begin();
    // set the resolution to 9 bit per device
    sensors.setResolution(dsEpraAddr, TEMPERATURE_PRECISION);
    sensors.setResolution(dsAmbientAddr, TEMPERATURE_PRECISION);
    sensors.setResolution(dsWaterAddr, TEMPERATURE_PRECISION);
}

String DallasSearch4web()
{
    byte dallasCount = sensors.getDS18Count();

    String result = "OneWire Dallas Sensor Addresses:\r\n";

    for (byte i = 0; i < dallasCount; i++)
    {
        result += "Device " + String(i);
        result += ": ";
        DeviceAddress addr;
        if (!sensors.getAddress(addr, i))
        {
            result += "Unable to find address";
        }
        else
        {
            result += printAddress(addr);
        }
        result += "\r\n";
    }

    return result;
}

void DallasSetup()
{
    sensors.begin();
    /*if (!sensors.getAddress(dsEpraAddr, 0))
        Serial.println("Unable to find address for Device 0");
    if (!sensors.getAddress(dsAmbientAddr, 1))
        Serial.println("Unable to find address for Device 1");
    // show the addresses we found on the bus
    Serial.print("Device 0 Address: ");
    printAddress(dsEpraAddr);
    Serial.println();
    Serial.print("Device 1 Address: ");
    printAddress(dsAmbientAddr);
    Serial.println();*/

    // set the resolution to 9 bit per device
    sensors.setResolution(dsEpraAddr, TEMPERATURE_PRECISION);
    sensors.setResolution(dsAmbientAddr, TEMPERATURE_PRECISION);
    sensors.setResolution(dsWaterAddr, TEMPERATURE_PRECISION);

    /*Serial.print("Device 0 Resolution: ");
    Serial.print(sensors.getResolution(dsEpraAddr), DEC);
    Serial.println();
    Serial.print("Device 1 Resolution: ");
    Serial.print(sensors.getResolution(dsAmbientAddr), DEC);
    Serial.println();*/
}

// function to print a device address
String printAddress(DeviceAddress deviceAddress)
{
    String result = "";
    for (uint8_t i = 0; i < 8; i++)
    {
        if (i > 0)
            result += ", 0x";
        else
            result += "0x";
        // zero pad the address if necessary
        if (deviceAddress[i] < 16)
            result += "0";
        result += String(deviceAddress[i], HEX);
    }
    return result;
}

void CheckDallas()
{
    sensors.requestTemperatures();
    dsEpraTemp = sensors.getTempC(dsEpraAddr);
    dsAmbientTemp = sensors.getTempC(dsAmbientAddr);
    dsWaterTemp = sensors.getTempC(dsWaterAddr);
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