#include <stdlib.h>
#include <string.h>

#include "major.h"
#include "my_xmalloc.h"

#define SHOPT_OPT_LEN 7
#define SHOPT_OPT                                                              \
    {                                                                          \
        "dotglob", "expand_aliases", "extglob", "nocaseglob", "nullglob",      \
            "sourcepath", "xpg_echo"                                           \
    }

#define SHOPT_OPT_DEFAULT_VALUE                                                \
    {                                                                          \
        0, 0, 0, 0, 0, 1, 0                                                    \
    }

static void shopt_add_opt(struct major *mj, char *opt_name, int value)
{
    struct shopt_opt_list *sol =
        my_xcalloc(mj, 1, sizeof(struct shopt_opt_list));

    sol->name = opt_name;
    sol->value = value;

    if (!mj->shopt_opt)
        mj->shopt_opt = sol;
    else
    {
        struct shopt_opt_list *temp = mj->shopt_opt;
        while (temp->next)
            temp = temp->next;

        temp->next = sol;
    }
}

void shopt_init_list(struct major *mj)
{
    char *shopt_name[] = SHOPT_OPT;
    int default_value[] = SHOPT_OPT_DEFAULT_VALUE;
    for (int i = 0; i < SHOPT_OPT_LEN; i++)
    {
        shopt_add_opt(mj, shopt_name[i], default_value[i]);
    }
}

void shopt_free_list(struct major *mj)
{
    struct shopt_opt_list *sol = mj->shopt_opt;

    while (sol && sol->next)
    {
        sol = sol->next;
        free(mj->shopt_opt);
        mj->shopt_opt = sol;
    }
    free(mj->shopt_opt);
}

int shopt_set_opt(struct major *mj, char *opt_name, int value)
{
    if (!mj->shopt_opt)
        shopt_init_list(mj);

    struct shopt_opt_list *temp = mj->shopt_opt;

    while (temp)
    {
        if (!strcmp(opt_name, temp->name))
        {
            temp->value = value;
            break;
        }
        temp = temp->next;
    }

    return !temp;
}