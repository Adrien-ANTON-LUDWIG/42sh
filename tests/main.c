#define _GNU_SOURCE
#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>

#include "argument_handler.h"
#include "ast.h"
#include "custom_descriptor.h"
#include "lexer.h"
#include "parser.h"
#include "printer.h"

TestSuite(_42sh, .timeout = 15);
// LEXER
Test(_42SH, simple_lex_if)
{
    char *s = "if";
    struct major *mj = major_init();
    mj->file = createfrom_string(s);
    struct token *tk = lexer_build(mj);
    cr_assert_eq(strcmp("WORD_IF", token2string(tk)), 0);
    major_free(mj);
}

Test(_42SH, simple_lex_then)
{
    char *s = "then";
    struct major *mj = major_init();
    mj->file = createfrom_string(s);
    struct token *tk = lexer_build(mj);
    cr_assert_eq(strcmp("WORD_THEN", token2string(tk)), 0);
    major_free(mj);
}

Test(_42SH, simple_lex_else)
{
    char *s = "else";
    struct major *mj = major_init();
    mj->file = createfrom_string(s);
    struct token *tk = lexer_build(mj);
    cr_assert_eq(strcmp("WORD_ELSE", token2string(tk)), 0);
    major_free(mj);
}

Test(_42SH, simple_lex_fi)
{
    char *s = "fi";
    struct major *mj = major_init();
    mj->file = createfrom_string(s);
    struct token *tk = lexer_build(mj);
    cr_assert_eq(strcmp("WORD_FI", token2string(tk)), 0);
    major_free(mj);
}

Test(_42SH, empty_string_lexer)
{
    char *s = "";
    struct major *mj = major_init();
    mj->file = createfrom_string(s);
    struct token *tk = lexer_build(mj);
    cr_assert_eq(strcmp("WORD_EOF", token2string(tk)), 0);
    major_free(mj);
}

Test(_42SH, hard_lex_if)
{
    char *s = "echo first; if echo toto; then echo tata; fi; echo end";
    struct major *mj = major_init();
    mj->file = createfrom_string(s);

    struct token *tk = lexer_build(mj);
    cr_assert_eq(strcmp("WORD_COMMAND", token2string(tk)), 0);

    tk = lexer_build(mj);
    cr_assert_eq(strcmp("WORD_IF", token2string(tk)), 0);

    tk = lexer_build(mj);
    cr_assert_eq(strcmp("WORD_COMMAND", token2string(tk)), 0);

    tk = lexer_build(mj);
    cr_assert_eq(strcmp("WORD_THEN", token2string(tk)), 0);

    tk = lexer_build(mj);
    cr_assert_eq(strcmp("WORD_COMMAND", token2string(tk)), 0);

    tk = lexer_build(mj);
    cr_assert_eq(strcmp("WORD_FI", token2string(tk)), 0);

    tk = lexer_build(mj);
    cr_assert_eq(strcmp("WORD_COMMAND", token2string(tk)), 0);

    major_free(mj);
}

Test(_42SH, simple_redir_lex)
{
    char *s = "echo toto > file";
    struct major *mj = major_init();
    mj->file = createfrom_string(s);

    struct token *tk = lexer_build(mj);
    cr_assert_eq(strcmp("WORD_COMMAND", token2string(tk)), 0);

    tk = lexer_build(mj);
    cr_assert_eq(strcmp("WORD_REDIR", token2string(tk)), 0);

    major_free(mj);
}

Test(_42sh, custom_descriptor1)
{
    struct custom_FILE *f = createfrom_string(
        "Salut!\nJe suis un test\nEt je te veux du mal\nissou");
    char *buffer = malloc(128 * sizeof(char));
    char *savedbuffer = buffer;
    buffer = custom_fgets(buffer, 128, f);
    cr_expect_eq(strcmp(buffer, "Salut!\n"), 0);
    buffer = custom_fgets(buffer, 128, f);
    cr_expect_eq(strcmp(buffer, "Je suis un test\n"), 0);
    buffer = custom_fgets(buffer, 128, f);
    cr_expect_eq(strcmp(buffer, "Et je te veux du mal\n"), 0);
    buffer = custom_fgets(buffer, 128, f);
    cr_expect_eq(strcmp(buffer, "issou"), 0);
    char *newbuffer = custom_fgets(buffer, 128, f);
    cr_expect_null(newbuffer);
    free(savedbuffer);
    custom_fclose(f);
}

Test(_42sh, merge_commands)
{
    char *argv[] = { "Salut", "comment", "ça", "va" };
    char *merged = merge_arguments(4, argv);
    cr_assert_eq(strcmp("Salut comment ça va", merged), 0);
    free(merged);
}

Test(_42sh, merge_commands_first_empty)
{
    char *argv[] = { "", "Salut" };
    char *merged = merge_arguments(2, argv);
    cr_assert_eq(strcmp("Salut", merged), 0);
    free(merged);
}

Test(_42sh, merge_commands_empty)
{
    char *argv[] = { NULL };
    char *merged = merge_arguments(0, argv);
    cr_assert_null(merged);
    free(merged);
}

Test(_42sh, parser_simple_if)
{
    struct lexer *lex = lexer_build(NULL, "if echo test; then echo toto; fi");
    struct ast *ast = take_action(NULL, NULL, lex, lexer_pop_head(NULL, lex));
    cr_assert_eq(strcmp(ast->left->data->data->head->data, "echo"), 0);
    cr_assert_eq(strcmp(ast->left->data->data->tail->data, "test"), 0);
    cr_assert_eq(strcmp(ast->right->data->data->head->data, "echo"), 0);
    cr_assert_eq(strcmp(ast->right->data->data->tail->data, "toto"), 0);
    lexer_free(lex);
    ast_free(ast);
}

int main(int argc, char **argv)
{
    struct criterion_test_set *tests = criterion_initialize();

    int result = 0;
    if (criterion_handle_args(argc, argv, true))
        result = !criterion_run_all_tests(tests);
    criterion_finalize(tests);
    return result;
}
