#include "variables_substitution.h"

#include "b_utils.h"
#include "char_array_op.h"
#include "dollar.h"
#include "my_xmalloc.h"

#define NUMBER_OF_SPECIAL_VARIABLES 9
#define SPECIAL_VARIABLES                                                      \
    {                                                                          \
        "$@", "$*", "$?", "$#", "$$", "$RANDOM", "$UID", "$OLDPWD", "$IFS"     \
    }

typedef char *(*predicate)(struct major *, int *);

static predicate substitution[] = {
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
    free(str);

    return new_str;
}

static int get_index_func(char *temp)
{
    char *spec_var[] = SPECIAL_VARIABLES;

    int i = 0;
    for (; i < NUMBER_OF_SPECIAL_VARIABLES; i++)
        if (!strncmp(spec_var[i], temp, strlen(spec_var[i])))
            return i;
    return i;
}

static char *var_subs_in_string(struct major *mj, char *str)
{
    char *temp = str;
    int strong_quote = 0;
    while (temp && *temp)
    {
        char c = *temp;
        if (c == '\'')
        {
            strong_quote = !strong_quote;
            temp++;
        }

        else if (c == '$' && !strong_quote)
        {
            int i = get_index_func(temp);
            size_t index = temp - str;

            str = (i < NUMBER_OF_SPECIAL_VARIABLES)
                ? manage_string(mj, str, index, *(substitution[i]))
                : dollar_unknown(mj, str, index);
            temp = str;
        }
        else
            temp++;
    }
    return str;
}

char **variables_substitution(struct major *mj, struct list *list)
{
    if (!list)
        return NULL;

    char **argv = token_list_to_char_array(list);
    if (!mj)
    {
        my_soft_err(mj, 1,
                    "Variable_substitution: Major should exist at this point");
        return argv;
    }

    char **cmd = char_array_dup(mj, argv);
    int len = argv_len(cmd + 1);
    for (int i = 0; i <= len; i++)
    {
        if (strstr(cmd[i], "$"))
            cmd[i] = var_subs_in_string(mj, cmd[i]);
    }
    free(argv);
    return cmd;
}