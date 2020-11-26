#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "argument_handler.h"
#include "ast.h"
#include "custom_descriptor.h"
#include "execution.h"
#include "lexer.h"
#include "printer.h"

int main(int argc, char **argv)
{
    if (argc < 2)
        return 0;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            if (i + 1 == argc)
                errx(2, "-c: option requires an argument");

            struct major *mj = major_init();
            int from = get_index_command_string(i + 1, argc, argv);
            char *args = merge_arguments(argc - from, argv + from);
            struct lexer *lexer = lexer_build(mj, args);
            lexer_printer(lexer);
            free(args);
            return 0;
        }
        else if (strcmp(argv[i], "-O") == 0)
            return 0;
        else if (strcmp(argv[i], "+O") == 0)
            return 0;
        else
            errx(1, "Wait, that's illegal!");
    }
    return 0;
}
