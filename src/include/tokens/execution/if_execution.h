#ifndef IF_EXECUTION_H
#define IF_EXECUTION_H

#include "my_utils.h"
#include "tokens.h"

int execution_if(struct major *mj, struct token *tk);
int allow_son_execution_if(struct major *mj, struct token *tk, int rvalue);

#endif /* IF_EXECUTION_H */