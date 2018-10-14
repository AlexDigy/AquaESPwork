#include <DallasTemperature.h>

void DallasBegin();
void DallasSetup();
String DallasSearch4web();
String printAddress(DeviceAddress deviceAddress);
void printTemperature(DeviceAddress deviceAddress);
void printResolution(DeviceAddress deviceAddress);
void printData(DeviceAddress deviceAddress);