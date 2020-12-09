#include "parser.h"

#include <stdlib.h>

#include "ast.h"
#include "exec_ast.h"
#include "list.h"
#include "my_err.h"
#include "my_xmalloc.h"
#include "string_manipulation.h"
#include "tokens.h"

struct ast *add_single_command(struct major *mj, struct ast *ast,
                               struct token **tk)
{
    if (!ast)
    {
        ast = create_ast(mj, *tk);
        return ast;
    }
    struct token *and = my_xcalloc(mj, 1, sizeof(struct token));
    and->word = WORD_SUPERAND;
    struct ast *newast = create_ast(mj, and);
    newast->left = ast;
    if (tk)
        newast->right = create_ast(mj, *tk);
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
    else if ((*tk)->word == WORD_REDIR_R) // Ouais je suis con, ouais
        token_free(*tk);
    else if ((*tk)->word == WORD_WHILE || (*tk)->word == WORD_UNTIL)
        ast = parser_while(mj, ast, tk);
    else if ((*tk)->word == WORD_FOR)
        ast = parser_for(mj, ast, tk);
    else
        my_err(2, mj, "parser: take_action: syntax error");
    return ast;
}

struct ast *get_ast(struct major *mj, struct ast *ast, struct token **tk)
{
    ast = take_action(mj, ast, tk);
    while (is_operator(*tk))
        ast = parser_operator(mj, ast, tk);
    if (!(*tk)->data)
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
    }
    token_free(tk);
}