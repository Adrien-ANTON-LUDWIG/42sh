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
            char *args = make_command(argc, argv);
            printf("%s\n", args);
            free(args);
        }
        else if (strcmp(argv[i], "-O") == 0)
        {
            return 0;
        }
        else if (strcmp(argv[i], "+O") == 0)
        {
            return 0;
        }
    }
    return 0;
}
