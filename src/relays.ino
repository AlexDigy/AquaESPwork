int in1 = 5;              // Указываем, что вывод реле In1, подключен к реле цифровому выводу 5
int in2 = 6;              // Указываем, что вывод реле In2, подключен к реле цифровому выводу 6
 
void setup()
{
 pinMode(in1, OUTPUT);    // Установим вывод 5 как выход
 pinMode(in2, OUTPUT);    // Установим вывод 6 как выход
}
void loop()
{
 digitalWrite(in1, HIGH); // Включаем реле
 delay(2000);             // Ждем 2с
 digitalWrite(in1, LOW);  // Выключаем реле
 delay(2000);             // Ждем 2с
 digitalWrite(in2, HIGH); // Включаем реле
 delay(2000);             // Ждем 2с 
 digitalWrite(in2, LOW);  // Выключаем реле
}