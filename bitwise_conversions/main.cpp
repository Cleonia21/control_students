#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

class ABC
{
    unsigned int _abcBits; // Содержит буквы из исходной строки

public:

    ABC(std::string str)
    {
        _abcBits = 0;
        for (int i = 0; str[i] != '\0'; i++)
        {
            if (str[i] >= 'A' && str[i] <= 'Z' || str[i] >= 'a' && str[i] <= 'z') // Если текущий символ - заглавная буква
            {
                // Первый бит будет символизировать - 'A', 26 бит - 'Z'.
                // В правой части выражения после символа | в случае с 'A' получаем число в двоичной системе 0x00...0001
                // В случае с Z - 0x0000001000...00 (последние 6 битов лишние, 26ой бит - 1, остальные нули)
                // Оператор | добавляет в переменную алфавита еще одну еденицу, в нужной позиции (| - оператор сложения)
                if (str[i] >= 'A' && str[i] <= 'Z')
                    _abcBits = _abcBits | (1 << (str[i] - 'A'));
                if (str[i] >= 'a' && str[i] <= 'z')
                    _abcBits = _abcBits | (1 << (str[i] - 'a'));
            }
        }
    }

    // Перегрузка оператора ~. Позволяет получить в закодированном виде
    // отсутствующие буквы, без прямого доступа к переменной хранящей буквы из исходной строки
    unsigned int operator~()
    {
        return (~_abcBits);
    }

};

// Перегрузка оператора потока стандартного вывода
std::ostream	&operator<<( std::ostream &ostr, ABC &instance )
{
    // Используем перегрузку оператора ~
    unsigned int revertABC = ~instance;

    for (int i = 25; i >= 0; i--)
    {
        char c = 'A' + i;
        // Последовательно сравниваем алфавит с каждым битом (от 0x000...0001 до 0x0000001000...)
        // & - логическое И. Если в алфавите стоит еденица, в проверяемом месте, то условие выполнится
        if (revertABC & (1 << i)) 
            ostr << c;
    }
    return ostr;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Invalid input" << std::endl;
        return -1;
    }

    ABC a(argv[1]);
    std::cout << a << std::endl;
    std::cout << "ZYXWVUTSRQPONMLKJIHGFEDCBA" << std::endl;
    return 0;
}