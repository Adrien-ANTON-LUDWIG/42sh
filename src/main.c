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
#include "parser.h"
#include "printer.h"

/**
 * @brief It's the main function :)
 *
 * @param argc
 * @param argv
 * @return int
 */
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
            parser(mj, lexer);
            free(args);
            lexer_free(lexer);
            major_free(mj);
            return 0;
        }
        else if (strcmp(argv[i], "-O") == 0)
            continue;
        else if (strcmp(argv[i], "+O") == 0)
            continue;
        else
        {
            struct custom_FILE *file = custom_fopen(argv[i]);

            struct major *mj = major_init();
            char *content = custom_getfile(file);
            struct lexer *lexer = lexer_build(mj, content);
            parser(mj, lexer);
            custom_fclose(file);
            free(content);
            lexer_free(lexer);
            major_free(mj);
        }
    }
    return 0;
}
