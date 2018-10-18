#include <Arduino.h>

class AquaTime
{
  public:
    AquaT lamp1;
    AquaT lamp2;
    AquaT lamp3;
    AquaT lamp4;

    void SetTimes(String str)
    {
        lamp1.OnHour = 8;
        lamp1.OnMinute = 30;
        lamp1.OffHour = 17;
        lamp1.OffMinute = 00;
        lamp2.OnHour = 9;
        lamp2.OnMinute = 30;
        lamp2.OffHour = 17;
        lamp2.OffMinute = 30;

        lamp3.OnHour = 10;
        lamp3.OnMinute = 30;
        lamp3.OffHour = 16;
        lamp3.OffMinute = 00;
        lamp4.OnHour = 11;
        lamp4.OnMinute = 30;
        lamp4.OffHour = 16;
        lamp4.OffMinute = 30;

        if (str == "")
            return;

        // 08:30;17:00;09:30;17:30;10:30;16:00;11:30;16:30
        if (str.length != 47)
            return;

        lamp1.Parse(str.substring(0, 10));
        lamp2.Parse(str.substring(12, 22));
        lamp3.Parse(str.substring(24, 34));
        lamp4.Parse(str.substring(36, 46));
    }
};

class AquaT
{
  public:
    int OnHour;
    int OnMinute;
    int OffHour;
    int OffMinute;

    void Parse(String str)
    {
        OnHour = str.substring(0, 1).toInt();
        OnMinute = str.substring(3, 4).toInt();

        OffHour = str.substring(6, 7).toInt();
        OffMinute = str.substring(9, 10).toInt();
    }
    //08:30;17:00
    bool CheckLamp(unsigned long epochTime)
    {
        unsigned long hours = (epochTime % 86400L) / 3600;
        unsigned long minutes = (epochTime % 3600) / 60;

        if (hours < OnHour)
            return false;

        if (hours == OnHour && minutes < OnMinute)
            return false;

        if (hours > OffHour)
            return false;

        if (hours == OffHour && minutes >= OffMinute)
            return false;

        return true;
    }
};