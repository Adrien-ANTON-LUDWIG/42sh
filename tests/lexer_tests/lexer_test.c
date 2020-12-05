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

/**
 * @brief Main to test the lexer.
 *
 * Parse the options as the real main but does not parse nor execute. Instead,
 * it prints the result of the lexer.
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
            if (i + 1 >= argc)
                errx(2, "-c: option requires an argument");

            struct major *mj = major_init();
            int from = get_index_command_string(i + 1, argc, argv);
            char *args = merge_arguments(argc - from, argv + from);
            mj->file = createfrom_string(args);
            struct token *tk = get_next_token(mj);

            while (tk->word != WORD_EOF)
            {
                print_token(tk);
                token_free(tk);
                tk = get_next_token(mj);
            }
            print_token(tk);
            token_free(tk);
            free(args);
            major_free(mj);
            return 0;
        }
        else if (strcmp(argv[i], "-O") == 0)
            continue;
        else if (strcmp(argv[i], "+O") == 0)
            continue;
        else
        {
            struct major *mj = major_init();
            mj->file = custom_fopen(argv[i]);

            struct token *tk = get_next_token(mj);

            while (tk->word != WORD_EOF)
            {
                print_token(tk);
                token_free(tk);
                tk = get_next_token(mj);
            }
            print_token(tk);
            token_free(tk);
            major_free(mj);
        }
    }
    return 0;
}