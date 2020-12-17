#include "parser.h"

#include <stdlib.h>

#include "ast.h"
#include "exec_ast.h"
#include "list.h"
#include "my_err.h"
#include "my_xmalloc.h"
#include "string_manipulation.h"
#include "tokens.h"

void superand_creator(struct major *mj, struct ast **ast)
{
    if (*ast)
    {
        struct ast *newast = add_single_command(mj, *ast, NULL);
        *ast = newast;
    }
}

struct ast *add_single_command(struct major *mj, struct ast *ast,
                               struct token **tk)
{
    if (!ast)
    {
        ast = create_ast(mj, *tk);
        if ((*tk)->word != WORD_COMMAND)
            *tk = get_next_token(mj);
        return ast;
    }
    struct token *and = my_xcalloc(mj, 1, sizeof(struct token));
    and->word = WORD_SUPERAND;
    struct ast *newast = create_ast(mj, and);
    newast->left = ast;
    if (tk)
        newast->right = create_ast(mj, *tk);
    if (tk && (*tk)->word != WORD_COMMAND)
        *tk = get_next_token(mj);
    return newast;
}

struct ast *take_action(struct major *mj, struct ast *ast, struct token **tk)
{
    if ((*tk)->word == WORD_IF)
        ast = parser_if(mj, ast, tk);
    else if ((*tk)->word == WORD_WORD)
        ast = parser_word(mj, ast, tk);
    else if ((*tk)->word == WORD_COMMAND)
        ast = add_single_command(mj, ast, tk);
    else if ((*tk)->word == WORD_WHILE || (*tk)->word == WORD_UNTIL)
        ast = parser_while(mj, ast, tk);
    else if ((*tk)->word == WORD_FOR)
        ast = parser_for(mj, ast, tk);
    else if ((*tk)->word == WORD_FUNCTION)
        ast = parser_function(mj, ast, tk, get_next_token(mj));
    else if ((*tk)->word == WORD_ASSIGNMENT)
        ast = add_single_command(mj, ast, tk);
    else if ((*tk)->word == WORD_NEWLINE)
    {
        token_free(*tk);
        *tk = get_next_token(mj);
    }
    else
        my_err(2, mj, "parser: take_action: syntax error");
    return ast;
}

struct ast *get_ast(struct major *mj, struct ast *ast, struct token **tk)
{
    ast = take_action(mj, ast, tk);
    while (is_operator(*tk))
        ast = parser_operator(mj, ast, tk);

    if (!(*tk)->data
        && ((*tk)->word == WORD_SEMIC || (*tk)->word == WORD_NEWLINE
            || (*tk)->word == WORD_EOF))
    {
        token_free(*tk);
        *tk = get_next_token(mj);
    }

    return ast;
}

void parser(struct major *mj)
{
    struct token *tk = get_next_token(mj);
    struct ast *ast = NULL;
    while (tk->word != WORD_EOF)
    {
        ast = get_ast(mj, ast, &tk);
        exec_ast(mj, ast);
        ast_free(ast);
        ast = NULL;
        mj->break_counter = 0;
    }
    token_free(tk);
}