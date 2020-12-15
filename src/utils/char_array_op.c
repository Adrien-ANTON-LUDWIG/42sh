#define _POSIX_C_SOURCE 200809L
#include "char_array_op.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "b_utils.h"
#include "my_xmalloc.h"

char **token_list_to_char_array(struct list *l)
{
    char **tab = my_xcalloc(NULL, l->size + 1, sizeof(char **));

    struct list_item *cpy = l->head;
    for (size_t i = 0; cpy && i < l->size; i++)
    {
        tab[i] = cpy->data;
        cpy = cpy->next;
    }
    return tab;
}

char **char_array_dup(struct major *mj, char **argv)
{
    if (!argv)
        return NULL;

    int len = argv_len(argv);
    char **tab = my_xcalloc(mj, len + 1, sizeof(char **));

    for (int i = 0; i < len; i++)
    {
        tab[i] = strdup(argv[i]);
    }
    tab[len] = NULL;

    return tab;
}

void char_array_free(char **argv)
{
    for (int i = 0; i < argv_len(argv); i++)
        free(argv[i]);
    free(argv);
}