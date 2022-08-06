#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Invalid input" << std::endl;
        return (-1);
    }

    std::string readFile = argv[1];
    size_t partNum = atoi(argv[2]);

    if (partNum == 0)
    {
        std::cout << "Invalid input" << std::endl;
        return (-1);
    }

    std::ifstream rf; // Переменная для чтения из файла

    rf.open(readFile, std::ios::in | std::ios::binary);
    if (!rf)
    {
        std::cout << "Open file error" << std::endl;
        return (-1);
    }

    std::streamsize readFileSize = 0;
    char c[1];
    while(rf) // Считаем количество символов в файле
    {
        rf.read(c, 1);
        if (rf)
            readFileSize++;
    }
    rf.close();

    if (readFileSize % partNum != 0)
    {
        std::cout << "Invalid input" << std::endl;
        return (-1);
    }

    // Что бы переместить каретку в начало файла просто закрываем и открываем его заново
    rf.open(readFile, std::ios::in | std::ios::binary); 
    if (!rf)
    {
        std::cout << "Open file error" << std::endl;
        return (-1);
    }

    std::ofstream wf; // Переменная для записи в файлы
    std::streamsize  writeFileSize = readFileSize / partNum;

    char str[writeFileSize];
    for (int i = 0; i < partNum; i++)
    {
        // В первом аргументе не учитывается расширение файла
        // Из исходного 1.txt будут созданы 1.txt1, 1.txt2 и т.д.
        wf.open(readFile + std::to_string(i + 1), std::ios::out | std::ios::binary);
        if (!wf)
        {
            rf.close();
            std::cout << "Open file error" << std::endl;
            return (-1);
        }
        rf.read(str, writeFileSize);
        if (rf)
            wf.write(str, writeFileSize);

        // // Если это последний файл-кусок, то в исходном файле может остаться хвостик
        // // К примеру, дан исходный файл размером 8, разбиваем его на 3 куска
        // // Получится 3 файла по 2 символа и еще 2 лишних символа в конце.
        // // Вот этот символ здесь дописываем в конец последнего файла
        // if (i + 1 == partNum)
        // {
        //     while(rf)
        //     {
        //         rf.read(c, 1);
        //         if (rf)
        //             wf.write(c, 1);
        //     }
        // }
        wf.close();
    }
    rf.close();
    return (0);
}
