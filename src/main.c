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
 * @brief Does nothing... yet
 *
 * @param i
 * @param argv
 */
void shopt_options(int *i, char **argv)
{
    while (!strcmp(argv[*i], "-O") || !strcmp(argv[*i], "+O"))
    {
        if (argv[*i][0] == '-')
            *i += 1;
        else
            *i += 1;
        *i += 1;
    }
}

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

    int i = 1;
    shopt_options(&i, argv);

    struct major *mj = major_init();
    int from = get_index_command_string(i, argc, argv);

    if (strcmp(argv[i], "-c") == 0)
    {
        if (i + 1 == argc)
            errx(2, "-c: option requires an argument");

        char *args = merge_arguments(argc - from, argv + from);
        mj->file = createfrom_string(args);
        mj->rvalue = 0;
        free(args);
        parser(mj);
        int rvalue = mj->rvalue;
        major_free(mj);
        return rvalue;
    }
    else
    {
        struct custom_FILE *file = custom_fopen(argv[from]);
        mj->file = file;
        parser(mj);
    }
    major_free(mj);

    return 0;
}