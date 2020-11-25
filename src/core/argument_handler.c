#include <stdlib.h>
#include <string.h>

char *make_command(int argc, char **argv)
{
    int size = 512;
    char *result = malloc(size * sizeof(char));
    int index = 0;
    for (int i = 2; i < argc; i++)
    {
        int len = strlen(argv[i]);
        if (index + len + 1 >= size)
        {
            result = realloc(result, size * 2);
            size *= 2;
        }
        if (i != 2)
        {
            result[index] = ' ';
            index++;
        }
        strcpy(result + index, argv[i]);
        index += len;
    }
    return result;
}