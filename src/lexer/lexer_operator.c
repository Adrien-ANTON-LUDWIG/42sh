#include "lexer_operator.h"

#include <string.h>

#include "lexer_utils.h"
#include "structures.h"

int next_is_operator(struct major *mj)
{
    size_t temp = mj->file->lexer_index;
    char *s = get_operator(mj);
    mj->file->lexer_index = temp;
    int rvalue = 0;

    if (!s)
        return 0;

    if (strcmp(s, "&&") == 0 || strcmp(s, "||") == 0 || strcmp(s, "|") == 0
        || strcmp(s, "&") == 0)
        rvalue = 1;

    free(s);

    return rvalue;
}