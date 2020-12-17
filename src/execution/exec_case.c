#include "exec_case.h"

#include <string.h>

#include "ast.h"
#include "char_array_op.h"
#include "exec_ast.h"
#include "execution.h"
#include "my_err.h"
#include "tokens.h"
#include "variables_substitution.h"

int exec_case(struct major *mj, struct ast *ast)
{
    char **arr = variables_substitution(mj, ast->data->data);
    char *str = arr[1];

    while (ast)
    {
        if (!ast->left)
        {
            char_array_free(arr);
            return 0;
        }
        struct list *list = ast->left->data->data;
        char **pattern = variables_substitution(mj, list);
        int len = list->size;

        for (int i = 0; i < len; i++)
        {
            if (!strcmp(str, pattern[i]))
            {
                char_array_free(arr);
                char_array_free(pattern);

                return exec_ast(mj, ast->right);
            }
        }

        char_array_free(pattern);
        ast = ast->middle;
    }

    char_array_free(arr);
    return 0;
}