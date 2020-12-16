#define _POSIX_C_SOURCE 200809L
#include <string.h>

#include "major.h"
#include "my_err.h"
#include "my_xmalloc.h"

static int is_variable(char *str)
{
    if (str && str[0] == '$')
        return 1;
    return 0;
}

char *get_variable_name(struct major *mj, char *var)
{
    char *name = strdup(var);
    char *name_save = name;
    name++;

    if (name[0] == '{')
        name++;

    int end = strlen(name);
    if (end == 0)
        my_err(2, mj, "Bad variable declaration");

    if (name[end - 1] == '}')
        name[end - 1] = 0;
    char *newname = strdup(name);
    free(name_save);

    return newname;
}

char *get_variable_value(struct major *mj, char *name)
{
    struct varlist *current = mj->variables;
    while (current)
    {
        if (!strcmp(current->name, name))
            return current->value;
        current = current->next;
    }
    return "";
}

char **replace_variables(struct major *mj, char **command)
{
    for (int i = 0; command[i]; i++)
    {
        if (is_variable(command[i]))
        {
            char *name = get_variable_name(mj, command[i]);
            char *value = get_variable_value(mj, name);
            if (value)
                command[i] = value;
            else
                command[i] = "";
        }
    }
    return command;
}

void variable_list_free(struct major *mj)
{
    struct varlist *current = mj->variables;
    while (current)
    {
        struct varlist *next = current->next;
        free(current->name);
        free(current->value);
        free(current);
        current = next;
    }
    mj->variables = NULL;
}