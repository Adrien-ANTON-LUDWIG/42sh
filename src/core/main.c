#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
        return 0;

    printf("%s\n", argv[1]);
    return 0;
}
