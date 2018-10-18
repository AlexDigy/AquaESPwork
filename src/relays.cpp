#include <Arduino.h>
int in1 = 13; // Указываем, что вывод реле In1, подключен к реле цифровому выводу
int in2 = 12; // Указываем, что вывод реле In2, подключен к реле цифровому выводу
int in3 = 14;
int in4 = 16;

void SetupRelays()
{
    pinMode(in1, OUTPUT); // Установим вывод как выход
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    SetOutput(1, true);
    SetOutput(2, true);
    SetOutput(3, true);
    SetOutput(4, true);
}

void SetOutput(byte number, bool value)
{
    int num = 1;
    switch (number)
    {
    case 1:
        num = in1;
        break;
    case 2:
        num = in2;
        break;
    case 3:
        num = in3;
        break;
    case 4:
        num = in4;
        break;
    }
    if (value)
        digitalWrite(num, HIGH); // Включаем реле
    else
        digitalWrite(num, LOW); // Выключаем реле
}

bool tekval = true;
byte iter = 1;

void sample()
{
    SetOutput(iter++, tekval);

    if (iter > 4)
    {
        iter = 1;
        tekval = !tekval;
    }
}