#include "variables_substitution.h"

#include "dollar.h"
#include "my_xmalloc.h"

#define NUMBER_OF_SPECIAL_VARIABLES 9
#define SPECIAL_VARIABLES                                                      \
    {                                                                          \
        "$@", "$*", "$?", "$#", "$$", "$RANDOM", "$UID", "$OLDPWD", "$IFS"     \
    }

char *(*substitution[9])() = {
    dollar_at,    dollar_star,   dollar_interrogation,
    dollar_sharp, dollar_dollar, dollar_random,
    dollar_uid,   dollar_oldpwd, dollar_ifs
};

static char *manage_string(struct major *mj, char *str, size_t index,
                           char *(*predicate)(struct major *mj, int *len_var))
{
    int len_var = 0;
    char *str_to_append = predicate(mj, &len_var);
    char *str_after_var = str + index + len_var;
    size_t len = index + strlen(str_to_append) + strlen(str_after_var) + 1;

    char *new_str = my_xcalloc(mj, 1, len * sizeof(char));

    new_str = strncpy(new_str, str, index);
    new_str = strcat(new_str, str_to_append);
    new_str = strcat(new_str, str_after_var);
    new_str[strlen(new_str)] = '\0';
    free(str_to_append);

    return new_str;
}

static char *var_subs_in_string(struct major *mj, char *str)
{
    char *temp = str;
    char *str_saved_for_free = str;
    char *spec_var[] = SPECIAL_VARIABLES;

    substitution[0] = dollar_at;
    substitution[1] = dollar_star;
    substitution[2] = dollar_interrogation;
    substitution[3] = dollar_sharp;
    substitution[4] = dollar_dollar;
    substitution[5] = dollar_random;
    substitution[6] = dollar_uid;
    substitution[7] = dollar_oldpwd;
    substitution[8] = dollar_ifs;

    while (temp && (temp = strstr(temp, "$")))
    {
        for (int i = 0; i < NUMBER_OF_SPECIAL_VARIABLES; i++)
        {
            if (!strncmp(spec_var[i], temp, strlen(spec_var[i])))
            {
                str = manage_string(mj, str, temp - str, *(substitution[i]));
                break;
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

    if (!mj)
    {
        printf("struct major should exist at this point");
        return list;
    }

    struct list_item *temp = list->head;
    while (temp)
    {
        if (strstr(temp->data, "$"))
            temp->data = var_subs_in_string(mj, temp->data);
        temp = temp->next;
    }
    return list;
}