#include <DallasTemperature.h>

extern float dsEpraTemp;
extern float dsAmbientTemp;
extern float dsWaterTemp;

void DallasBegin();
void DallasSetup();
String DallasSearch4web();
String printAddress(DeviceAddress deviceAddress);
void CheckDallas();
void printTemperature(DeviceAddress deviceAddress);
void printResolution(DeviceAddress deviceAddress);
void printData(DeviceAddress deviceAddress);