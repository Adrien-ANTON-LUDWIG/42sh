#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "lexer.h"
#include "my_utils.h"

/**
 * @brief Converts the command from struct list to a char **
 *
 * @param l
 * @return char**
 */
char **token_list_to_char_array(struct list *l)
{
    char **tab = malloc(sizeof(char **) * l->size);

    struct list_item *cpy = l->head;
    for (size_t i = 0; cpy && i < l->size; i++)
    {
        tab[i] = cpy->data;
        cpy = cpy->next;
    }
    return tab;
}

/**
 * @brief Executes a command
 *
 * @param args
 * @return int
 */
int run_command(char **args)
{
    int pid = fork();
    if (!pid)
    {
        execvp(args[0], args);
        exit(127);
    }
    int rvalue = 0;
    waitpid(pid, &rvalue, 0);
    rvalue = WEXITSTATUS(rvalue);
    if (rvalue == 127)
        fprintf(stderr, "Command not found: %s", args[0]);
    return rvalue;
}