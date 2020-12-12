#include "variables_substitution.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "argument_handler.h"
#include "b_utils.h"
#include "my_xmalloc.h"

#define NUMBER_OF_SPECIAL_VARIABLES 9
#define SPECIAL_VARIABLES                                                      \
    {                                                                          \
        "$@", "$*", "$?", "$#", "$$", "$RANDOM", "$UID", "$OLDPWD", "$IFS"     \
    }

static char *dollar_at(struct major *mj, int *len_var)
{
    *len_var = 2;
    int argc = argv_len(mj->arguments);
    char *str = merge_arguments(argc, mj->arguments);
    return str;
}

char *(*substitution[1])() = { dollar_at };

static char *manage_string(struct major *mj, char *str, size_t index,
                           char *(*predicate)(struct major *mj, int *len_var))
{
    int len_var = 0;
    char *str_to_append = predicate(mj, &len_var);
    char *str_after_var = str + index + len_var;

    char *new_str = my_xcalloc(
        mj, 1, index + strlen(str_to_append) + strlen(str_after_var) + 1);

    new_str = strncpy(new_str, str, index);
    new_str = strcat(new_str, str_to_append);
    new_str = strcat(new_str, str_after_var);
    new_str[strlen(new_str)] = '\0';
    free(str);

    return new_str;
}

static char *var_subs_in_string(struct major *mj, char *str)
{
    char *temp = str;
    char *str_saved_for_free = str;
    char *spec_var[] = SPECIAL_VARIABLES;
    substitution[0] = dollar_at;
    while (temp && (temp = strstr(temp, "$")))
    {
        for (int i = 0; i < NUMBER_OF_SPECIAL_VARIABLES; i++)
        {
            if (!strncmp(spec_var[i], temp, strlen(spec_var[i])))
            {
                str = manage_string(mj, str, temp - str, *(substitution[0]));
            }
        }
        temp++;
    }

    free(str_saved_for_free);
    return str;
}

struct list *variables_substitution(struct major *mj, struct list *list)
{
    if (!list)
        return list;

    struct list_item *temp = list->head;

    while (mj && temp && list)
    {
        if (strstr(temp->data, "$"))
            temp->data = var_subs_in_string(mj, temp->data);
        temp = temp->next;
    }

    return list;
}