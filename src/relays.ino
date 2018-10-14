#include <Arduino.h>
int in1 = 13;              // Указываем, что вывод реле In1, подключен к реле цифровому выводу 
int in2 = 12;              // Указываем, что вывод реле In2, подключен к реле цифровому выводу 
 
void SetupRelays()
{
 pinMode(in1, OUTPUT);    // Установим вывод 5 как выход
 pinMode(in2, OUTPUT);    // Установим вывод 6 как выход
}
void sample()
{
 digitalWrite(in1, HIGH); // Включаем реле
 delay(2000);             // Ждем 2с
 digitalWrite(in1, LOW);  // Выключаем реле
 delay(2000);             // Ждем 2с
 digitalWrite(in2, HIGH); // Включаем реле
 delay(2000);             // Ждем 2с 
 digitalWrite(in2, LOW);  // Выключаем реле
}