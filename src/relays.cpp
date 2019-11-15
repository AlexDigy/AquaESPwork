#include <Arduino.h>
#include "relays.h"

int in1 = 13; // Указываем, что вывод реле In1, подключен к реле цифровому выводу
int in2 = 12; // Указываем, что вывод реле In2, подключен к реле цифровому выводу
int in3 = 14;
//int in4 = 2; // был 16. Второй нельзя использовать, т.к. дергается постоянно логгером
int fun = 16; // вентилятор

void SetupRelays()
{
    pinMode(in1, OUTPUT); // Установим вывод как выход
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    //pinMode(in4, OUTPUT);
    pinMode(fun, OUTPUT);

    SetLampState(1, false);
    SetLampState(2, false);
    SetLampState(3, false);
    //SetLampState    (4, true);
    //SetLampState(5, false);
    SetFun(0);
}

void SetLampState(byte number, bool value)
{
    int num = 0;
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
        /*case 4:
        num = in4;
        break;*/
    }
    if (num < 1 && num > 3)
        return;

    if (value)
        digitalWrite(num, LOW); // Включаем реле
    else
        digitalWrite(num, HIGH); // Выключаем реле
}

// value 0 to 100 %
void SetFun(byte value)
{
    /*if (value == 0)
        digitalWrite(fun, LOW);
    else
        digitalWrite(fun, HIGH);*/

    // вентилятор стартует с 20%
    int val = 200 + (1023 - 200) * value / 100;

    if (value == 0)
        val = 0;

    analogWrite(fun, val);
}
