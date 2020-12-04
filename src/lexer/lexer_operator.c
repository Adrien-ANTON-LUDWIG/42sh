#include "structures.h"
#include "lexer_utils.h"
#include "lexer_operator.h"

#include <string.h>

int next_is_operator(struct major *mj)
{
    size_t temp = mj->file->lexer_index;
    char *s = get_word(mj);
    mj->file->lexer_index = temp;

    if (!s)
        return 0;

    if (strcmp(s, "&&") == 0 || strcmp(s, "||") == 0)
        return 1;

    free(s);

    return 0;
}