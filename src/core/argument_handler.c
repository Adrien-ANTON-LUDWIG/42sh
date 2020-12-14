#include "argument_handler.h"

#include <err.h>
#include <stdlib.h>
#include <string.h>

int get_index_command_string(int index, int argc, char *argv[])
{
    while (index < argc && argv[index][0] == '-')
    {
        if (strlen(argv[index]) >= 2 && argv[index][1] == '-')
            errx(2, "--: invalid option");
        index++;
    }
    return index;
}

char *merge_arguments(int argc, char **argv)
{
    if (argc <= 0)
        return NULL;
    int size = 4096;
    char *result = malloc(size * sizeof(char));
    int index = 0;
    for (int i = 0; i < argc; i++)
    {
        int len = strlen(argv[i]);
        while (index + len + 1 >= size)
        {
            result = realloc(result, size * 2);
            size *= 2;
        }
        if (i != 0 && strlen(argv[i - 1]))
        {
            result[index] = ' ';
            index++;
        }
        strcpy(result + index, argv[i]);
        index += len;
    }
    return result;
}