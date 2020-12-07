#include "tokens.h"

int execution_if(struct major *mj, struct token *tk)
{
    if (mj && tk)
        return 0;
    return 0;
}

int allow_son_execution_if(struct major *mj, struct token *tk, int rvalue)
{
    if (mj && tk)
        return rvalue == 0;
    if (rvalue == 0)
        return 1;
    return 0;
}