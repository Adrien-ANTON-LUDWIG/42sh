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
    new->name = strdup(name);
    new->value = strdup(value);
    return new;
}

void variable_declare(struct major *mj, char *name, char *value)
{
    if (!mj->variables)
        mj->variables = varlist_init(mj, name, value);
    else
    {
        struct varlist *current = mj->variables;
        int declared = 0;
        while ((declared = !strcmp(name, current->name)) || current->next)
        {
            if (declared)
            {
                free(current->value);
                current->value = strdup(value);
                return;
            }
            current = current->next;
        }
        current->next = varlist_init(mj, name, value);
    }
}