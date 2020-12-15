#include "major.h"

#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "custom_descriptor.h"
#include "list.h"
#include "my_xmalloc.h"
#include "shopt.h"
#include "variables.h"

struct major *major_init(void)
{
    return my_xcalloc(NULL, 1, sizeof(struct major));
}

void major_free(struct major *mj)
{
    if (mj->shopt_opt)
        free_shopt_opt_list(mj);

    struct funclist *fl = mj->flist;

    while (fl)
    {
        struct funclist *next = fl->next;
        ast_free(fl->ast);
        free(fl);
        fl = next;
    }
    variable_list_free(mj);
    custom_fclose(mj->file);
    free(mj);
}

static int replace_function(struct funclist *current, struct funclist *new,
                            struct ast *newast)
{
    ast_free(current->ast);
    free(new);
    current->ast = newast;
    return 0;
}

int add_to_funclist(struct major *mj, struct ast *func)
{
    struct funclist *new = my_xcalloc(mj, 1, sizeof(struct funclist));
    struct funclist *current = mj->flist;
    struct ast *newast = create_ast(mj, func->data);

    func->data = NULL;
    newast->left = func->left;
    func->left = NULL;
    newast->right = func->right;
    func->right = NULL;
    new->ast = newast;

    char *funcname = newast->data->data->head->data;

    while (current && current->next)
    {
        if (!strcmp(current->ast->data->data->head->data, funcname))
            return replace_function(current, new, newast);

        current = current->next;
    }
    if (!current)
        mj->flist = new;
    else
    {
        if (!strcmp(current->ast->data->data->head->data, funcname))
            return replace_function(current, new, newast);

        current->next = new;
    }

    return 0;
}

struct major *major_copy(struct major *mj)
{
    struct major *newmajor = my_xmalloc(mj, sizeof(struct major));
    memcpy(newmajor, mj, sizeof(struct major));
    return newmajor;
}