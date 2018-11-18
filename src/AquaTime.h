#include <Arduino.h>

class AquaT
{
  public:
    int OnHour;
    int OnMinute;
    int OffHour;
    int OffMinute;

    void Parse(String str);
    bool CheckLamp(unsigned long epochTime);
};