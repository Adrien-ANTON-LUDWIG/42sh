#include <stdlib.h>

#include "ast.h"
#include "major.h"
#include "my_err.h"
#include "variable_declaration.h"

int assign_variable(struct major *mj, struct ast *ast)
{
    if (!ast)
        return my_soft_err(
            mj, 2, "assign_variable: Could not assign variable: No ast");
    char **variable_data = get_assignment_data(mj, ast->data);
    variable_declare(mj, variable_data[0], variable_data[1]);
    free(variable_data);
    return 0;
}