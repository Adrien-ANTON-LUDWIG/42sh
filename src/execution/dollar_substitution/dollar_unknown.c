#include "argument_handler.h"
#include "b_utils.h"
#include "dollar.h"
#include "string_manipulation.h"

char *dollar_unknown(struct major *mj, char *str, int index)
{
    int len_var = 1;

    char *stop = NULL;

    if (str[len_var] == '}')
    {
        stop = "}";
        len_var++;
    }
    else
    {
        stop = " $\t\n";
    }

    while (is_not_in(str[index + len_var], stop))
        len_var++;

    char *str_after_var = str + index + len_var;
    size_t len = index + strlen(str_after_var) + 1;

    char *new_str = my_xcalloc(mj, 1, len * sizeof(char));

    new_str = strncpy(new_str, str, index);
    new_str = strcat(new_str, str_after_var);
    new_str[strlen(new_str)] = '\0';

    return new_str;
}