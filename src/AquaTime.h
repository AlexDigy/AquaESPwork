#include "Arduino.h"

class AquaT
{
  public:
    unsigned int OnHour;
    unsigned int OnMinute;
    unsigned int OffHour;
    unsigned int OffMinute;

    void Parse(String str);
    bool CheckLamp(unsigned long epochTime);
};

class AquaTime
{
  public:
    AquaT lamp1;
    AquaT lamp2;
    AquaT lamp3;

    void SetTimes(String str);
};