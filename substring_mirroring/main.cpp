#include <iostream>
#include <list>

int main(int argc, char **argv)
{
    std::list<char> myList;

    if (argc != 2)
    {
        std::cout << "Invalid input" << std::endl;
        return (-1);
    }
    
    int tildaNum = 0;
    for (int i = 0; argv[1][i]; i++)
    {
        if (argv[1][i] == '~')
            tildaNum++;
        myList.push_back(argv[1][i]);
    }

    if (tildaNum != 2)
    {
        std::cout << "Invalid input" << std::endl;
        return (-1);
    }

    std::list<char>::iterator tilda = myList.end();
    
    while (*tilda != '~' && tilda != myList.begin()) // Ищем вторую тильду, если считать с начала строки
        tilda--;

    std::list<char>::iterator tmp = tilda;
    while (--tmp != myList.begin()) // Выводим левую половину задом на перед
        std::cout << *tmp;

    while (++tilda != myList.end()) // Выводим правую половину в первозданном виде
        std::cout << *tilda;

    std::cout << std::endl;

    return (0);
}