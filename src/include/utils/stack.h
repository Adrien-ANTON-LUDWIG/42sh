#ifndef STACK_H
#define STACK_H

#include "ast.h"
#include "major.h"

struct stack
{
    struct stack *next;
    struct ast *ast;
};

struct stack *stack_add(struct major *mj, struct stack *q, struct ast *ast);
struct ast *stack_pop(struct major *mj, struct stack **q);

#endif