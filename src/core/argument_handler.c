#include <stdlib.h>
#include <string.h>

/**
 * @brief Merges argv into a single string
 *
 * @param argc
 * @param argv
 * @return char*
 */
char *merge_arguments(int argc, char **argv)
{
    if (argc <= 0)
        return NULL;
    int size = 512;
    char *result = malloc(size * sizeof(char));
    int index = 0;
    for (int i = 0; i < argc; i++)
    {
        int len = strlen(argv[i]);
        if (index + len + 1 >= size)
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