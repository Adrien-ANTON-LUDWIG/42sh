#ifndef PIPE_H
#define PIPE_H

#include "ast.h"
#include "exec_ast.h"
#include "execution.h"
#include "my_err.h"

int exec_pipe(struct major *mj, struct ast *ast);

#endif