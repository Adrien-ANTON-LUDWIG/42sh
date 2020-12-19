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
#include "my_xmalloc.h"
#include "parser.h"
#include "printer.h"
#include "shopt.h"

static int run_command_line(char *argv[], int from, struct major *mj)
{
    mj->file = createfrom_string(mj, *(argv + from));
    mj->rvalue = 0;
    parser(mj);
    int rvalue = mj->rvalue;
    major_free(mj);
    return rvalue;
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
    int i = 0;

    if (argc > 1)
        i = 1;

    struct major *mj = major_init();
    i += shopt_options_argv(mj, argv + i);
    int from = get_index_command_string(i, argc, argv);
    mj->arguments = argv + from + 1;

    if (i < argc && strcmp(argv[i], "-c") == 0)
    {
        if (i + 1 == argc)
            errx(2, "-c: option requires an argument");

        return run_command_line(argv, from, mj);
    }
    else if (i < argc)
    {
        struct custom_FILE *file;
        if (argc >= 2)
            file = custom_fopen(mj, argv[from]);
        else
        {
            file = my_xcalloc(mj, 1, sizeof(struct custom_FILE));
            file->file = stdin;
            file->fd = stdin->_fileno;
        }
        mj->file = file;
        parser(mj);
    }

    int rvalue = mj->rvalue;
    major_free(mj);
    return rvalue;
}