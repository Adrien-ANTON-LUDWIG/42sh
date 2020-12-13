#include "stack.h"

#include <stdlib.h>

#include "ast.h"
#include "major.h"
#include "my_xmalloc.h"

struct stack *stack_add(struct major *mj, struct stack *s, struct ast *ast)
{
    struct stack *new = my_xcalloc(mj, 1, sizeof(struct stack));
    new->ast = ast;
    new->next = s;

    return new;
}

struct ast *stack_pop(struct major *mj, struct stack **s)
{
    if (!(*s) || !mj)
        return NULL;

    struct stack *first = *s;
    *s = (*s)->next;
    struct ast *depoped = first->ast;
    free(first);

    return depoped;
}