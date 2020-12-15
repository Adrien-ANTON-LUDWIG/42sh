#include "argument_handler.h"
#include "b_utils.h"
#include "dollar.h"
#include "string_manipulation.h"
#include "variables.h"

char *dollar_unknown(struct major *mj, char *str, int index)
{
    int len_var = 1;

    char *stop = NULL;

    if (str[index + 1] == '{')
    {
        stop = "}";
        len_var++;
    }
    else
        stop = " $\t\n";

    while (str[index + len_var] && is_not_in(str[index + len_var], stop))
        len_var++;

    char *str_after_var = str + index + len_var;
    if (*str_after_var == '}')
        str_after_var++;

    char *var_name = strndup(str + index, str_after_var - str - index);
    char *stripped_name = get_variable_name(mj, var_name);

    char *var_value = get_variable_value(mj, stripped_name);
    free(stripped_name);
    free(var_name);

    char *new_str =
        my_xcalloc(mj, index + strlen(var_value) + strlen(str_after_var) + 1,
                   sizeof(char));
    char *pre_variable = strndup(str, index);
    sprintf(new_str, "%s%s%s", pre_variable, var_value, str_after_var);
    free(pre_variable);

    return new_str;
}