#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct s_params
{
    char *file_name;
    int corn;
    int inversions_num;
}   t_params;


void bubbleSort(int *num, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = (size - 1); j > i; j--)
        {
            if (num[j - 1] > num[j])
            {
                int temp = num[j - 1];
                num[j - 1] = num[j];
                num[j] = temp;
            }
        }
    }
}

int read_flags(t_params *params, int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (i + 1 < argc) // Флаги не проверяются на повтор
            {
                if (argv[i][1] == 'f' && argv[i][2] == '\0')
                    params->file_name = argv[++i];
                else if (argv[i][1] == 's' && argv[i][2] == '\0')
                    params->corn = atoi(argv[++i]);
                else if (argv[i][1] == 'n' && argv[i][2] == '\0')
                {
                    params->inversions_num = atoi(argv[++i]);
                    if (params->inversions_num <= 0)
                        return -1;
                }
                else
                    return -1;
            }
            else
                return -1;
        }
        else
            return -1;
    }
    return 0;
}

int source_to_tmp(t_params *params)
{
    int fd, tmp_fd;
    fd = open(params->file_name, O_RDONLY, 0644); // Открываем только на чтение исходный файл
    tmp_fd = open("tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644); // Пересоздаем и открываем на запись временный файл
    if (fd == -1 || tmp_fd == -1)
        return (-1);

    char c;
    int retval;
    if (params->inversions_num < 0) // Если не указано число инверсий
    {
        while(1)
        {
            retval = read(fd, &c, 1); // Считываем 1 символ
            if (retval == -1) // В случае ошибки
                return (-1);
            if (retval == 0) // Если достигли конца файла
                break;
            c = ~c; // Инвертируем байт
            write(tmp_fd, &c, 1); // Записываем инвертированный байт во временный файл
        }
    }
    else
    {
        int *positions = (int *)malloc(sizeof(int) * params->inversions_num); // Массив позиций
        if (positions == NULL)
            return (-1);
        for (int i = 0; i < params->inversions_num; i++) // Заполняем массив пизиций случайными числами
            positions[i] = rand();
        bubbleSort(positions, params->inversions_num); // Сортируем позиции, что бы пройти файл линейно
        int pos_i = 0;
        for (int i = 0; ; i++) 
        {
            retval = read(fd, &c, 1); // Считываем 1 символ
            if (retval == -1) // В случае ошибки
                return (-1);
            if (retval == 0) // Если достигли конца файла
                break;
            if (positions[pos_i] == i)  // Инвертируем байт, если он стоит
            {                           // На одной из случайно созданных позиций
                c = ~c;
                if (pos_i < params->inversions_num) // Переходим к следующей позиции,
                    pos_i++;                // если еще не дошли до конца массива позиций
            }
            write(tmp_fd, &c, 1); // Записываем байт во временный файл
        }
        free (positions);
    }
    close(fd);
    close(tmp_fd);
    return (0);
}

int tmp_to_sourse(t_params *params)
{
    int fd = open(params->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);   // Открываем фалы по новой,
    int tmp_fd = open("tmp", O_RDONLY, 0644);    // только меняем их местами на запись и чтение

    if (fd == -1 || tmp_fd == -1)
        return (-1);
    char c;
    int retval;
    while(1) // Переписываем из временного файла в исходный результат
    {
        retval = read(tmp_fd, &c, 1);
        if (retval == -1)
            return (-1);
        if (retval == 0)
            break;
        write(fd, &c, 1);
    }
    close(fd);
    close(tmp_fd);
    return (0);
}

int main(int argc, char **argv)
{
    t_params params;
    params.file_name = NULL;
    params.corn = 0;
    params.inversions_num = -1;

    if (read_flags(&params, argc, argv) == -1)
    {
        printf("Invalid input\n");
        return (-1);
    }

    srand(params.corn);
    
    if (source_to_tmp(&params) == -1 || tmp_to_sourse(&params) == -1)
    {
        printf("Error while reading file\n");
        return (-1);
    }

    return (0);
}