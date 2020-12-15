#define _POSIX_C_SOURCE 200809L
#include <string.h>

#include "list.h"
#include "major.h"
#include "my_err.h"
#include "my_xmalloc.h"
#include "tokens.h"

static struct varlist *varlist_init(struct major *mj, char *name, char *value)
{
    struct varlist *new = my_xcalloc(mj, 1, sizeof(struct varlist));
    new->name = name;
    new->value = value;
    return new;
}

void variable_declare(struct major *mj, char *name, char *value)
{
    if (!mj->variables)
        mj->variables = varlist_init(mj, name, value);
    else
    {
        struct varlist *current = mj->variables;
        while (current->next)
        {
            if (!strcmp(current->name, name))
            {
                free(current->value);
                free(name);
                current->value = value;
                return;
            }
        }
        current->next = varlist_init(mj, name, value);
    }
}

char **get_assignment_data(struct major *mj, struct token *tk)
{
    char *str = tk->data->head->data;
    int i = 0;

    while (str[i] && str[i] != '=')
        i++;

    if (str[i] != '=')
        my_err(2, mj, "get_assignment_data: Bad declaration, expected '='");

    char *name = strndup(str, i);
    char *value = strndup(str + i + 1, strlen(str) - i - 1);
    char **result = my_xmalloc(mj, 2 * sizeof(char *));

    result[0] = name;
    result[1] = value;

    return result;
}