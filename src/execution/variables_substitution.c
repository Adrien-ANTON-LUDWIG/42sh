#define _POSIX_C_SOURCE 200809
#include "variables_substitution.h"

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "argument_handler.h"
#include "b_utils.h"
#include "my_xmalloc.h"

#define MAX_RANDOM 32767
#define NUMBER_OF_SPECIAL_VARIABLES 9
#define SPECIAL_VARIABLES                                                      \
    {                                                                          \
        "$@", "$*", "$?", "$#", "$$", "$RANDOM", "$UID", "$OLDPWD", "$IFS"     \
    }

static int first_time = 0;

static char *dollar_at(struct major *mj, int *len_var)
{
    *len_var = 2;
    int argc = argv_len(mj->arguments);
    char *str = merge_arguments(argc, mj->arguments);

    if (!str)
        str = strdup("");

    return str;
}

static char *dollar_star(struct major *mj, int *len_var)
{
    *len_var = 2;
    int argc = argv_len(mj->arguments);
    char *str = merge_arguments(argc, mj->arguments);

    if (!str)
        str = strdup("");

    return str;
}

static char *dollar_interrogation(struct major *mj, int *len_var)
{
    *len_var = 2;
    char *str = my_xcalloc(mj, 1, sizeof(char) * sizeof(int) + 1);

    sprintf(str, "%d", mj->rvalue);

    return str;
}

static char *dollar_sharp(struct major *mj, int *len_var)
{
    *len_var = 2;
    char *str = my_xcalloc(mj, 1, sizeof(char) * sizeof(int) + 1);
    sprintf(str, "%d", argv_len(mj->arguments));

    return str;
}

static char *dollar_dollar(struct major *mj, int *len_var)
{
    if (!mj)
        printf("struct major should exist at this point");

    *len_var = 2;
    char *str = my_xcalloc(mj, 1, sizeof(char) * sizeof(long int) + 1);

    int pid = getpid();

    sprintf(str, "%d", pid);

    return str;
}

static char *dollar_random(struct major *mj, int *len_var)
{
    if (!mj)
        printf("struct major should exist at this point");

    *len_var = 7;

    if (!first_time)
    {
        first_time = 1;
        srand(time(NULL));
    }

    int random = rand() % MAX_RANDOM;

    char *str = my_xcalloc(mj, 1, sizeof(char) * sizeof(int) + 10);

    sprintf(str, "%d", random);

    return str;
}

static char *dollar_uid(struct major *mj, int *len_var)
{
    *len_var = 4;
    if (!mj)
        printf("struct major should exist at this point");

    int pid = getuid();

    char *str = my_xcalloc(mj, 1, sizeof(char) * sizeof(int) + 1);
    sprintf(str, "%d", pid);
    return str;
}

static char *dollar_oldpwd(struct major *mj, int *len_var)
{
    if (!mj)
        printf("struct major should exist at this point");
    *len_var = 7;
    char *str = strdup(getenv("OLDPWD"));
    return str;
}

static char *dollar_ifs(struct major *mj, int *len_var)
{
    if (!mj)
        printf("struct major should exist at this point");

    *len_var = 4;
    char *str = strdup(" \t\n");
    return str;
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

    struct list_item *temp = list->head;

    while (mj && temp && list)
    {
        if (strstr(temp->data, "$"))
            temp->data = var_subs_in_string(mj, temp->data);
        temp = temp->next;
    }

    return list;
}