#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
        return 0;

     for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            return 1;
        }
        else if (strcmp(argv[i], "-O") == 0)
        {
            return 1;
        }
        else if (strcmp(argv[i], "+O") == 0)
        {
            return 1;
        }

    }
    return 0;
}
