#ifndef COMMAND_EXECUTION_H
#define COMMAND_EXECUTION_H

#include "my_utils.h"
#include "tokens.h"

int execution_command(struct major *mj, struct token *tk);
int allow_son_execution_command(struct major *mj, struct token *tk, int rvalue);

#endif /* COMMAND_EXECUTION_H */