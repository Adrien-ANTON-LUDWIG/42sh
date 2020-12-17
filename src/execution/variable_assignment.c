#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "list.h"
#include "major.h"
#include "my_err.h"
#include "my_xmalloc.h"
#include "variable_declaration.h"

char **get_assignment_data(struct major *mj, char *str, int i)
{
    char **result = my_xmalloc(mj, 2 * sizeof(char *));

    result[0] = str;
    result[1] = str + i + 1;

    return result;
}

int assign_variable(struct major *mj, struct ast *ast)
{
    if (!ast)
        return my_soft_err(
            mj, 2, "assign_variable: Could not assign variable: No ast");

    char *str = ast->data->data->head->data;
    int i = 0;

    while (str[i] && str[i] != '=')
        i++;

    if (str[i] != '=')
        my_err(2, mj, "get_assignment_data: Bad declaration, expected '='");

    str[i] = '\0';
    char **variable_data = get_assignment_data(mj, str, i);
    variable_declare(mj, variable_data[0], variable_data[1]);
    str[i] = '=';

    free(variable_data);
    return 0;
}