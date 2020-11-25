#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "argument_handler.h"
#include "custom_descriptor.h"

int main(int argc, char **argv)
{
    if (argc < 2)
        return 0;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            char *args = merge_arguments(argc - i - 1, argv + i + 1);
            printf("%s\n", args);
            free(args);
            return 0;
        }
        else if (strcmp(argv[i], "-O") == 0)
            return 0;
        else if (strcmp(argv[i], "+O") == 0)
            return 0;
        else
            errx(1, "Wait, that's illegal !");
    }
    return 0;
}
