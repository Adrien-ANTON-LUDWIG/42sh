#define _POSIX_C_SOURCE 200809L
#include "lexer_utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

static void skip_to_new_line(struct major *mj)
{
    if (mj->file->fd == CUSTOM_FD)
    {
        for (; mj->file->lexer_index < mj->file->len; mj->file->lexer_index++)
            if (mj->file->str[mj->file->lexer_index] == '\n')
            {
                mj->file->lexer_index++;
                break;
            }
    }
    else
    {
        char *tmp = custom_fgets(mj->file->str, BUFFER_SIZE, mj->file);

        if (!tmp)
        {
            free(mj->file->str);
            mj->file->str = NULL;
            return;
        }

        mj->file->lexer_index = 0;
        mj->file->len = strlen(mj->file->str);
    }
}

static int my_is_space(int c)
{
    return c == ' ' || c == '\t' || c == '\v';
}

static int c_is_redir(int c)
{
    return c == '<' || c == '>' || c == '-' || c == '|' || c == '&';
}

static int is_word(int c)
{
    return c != '\0' && !my_is_space(c) && c != ';' && c != '\n' && c != '\r'
        && c != '&' && c != '|' && c != '<' && c != '>';
}

static int is_operator(int c)
{
    return c == '&' || c == '|';
}

static void skip_class(int (*classifier)(int), struct major *mj)
{
    char *cursor = mj->file->str;
    if (!cursor)
        return;
    while (classifier(cursor[mj->file->lexer_index]))
    {
        mj->file->lexer_index++;

        if (mj->file->lexer_index >= mj->file->len)
        {
            if (mj->file->fd != CUSTOM_FD && mj->file->str
                && mj->file->lexer_index >= BUFFER_SIZE)
            {
                get_new_string(mj);
                if (mj->file->len == 0)
                    break;
            }
            else
                return;
        }
    }
}

char *get_word(struct major *mj)
{
    if (mj->file->str && mj->file->str[mj->file->lexer_index] == ';')
        mj->file->lexer_index++;

    skip_class(my_is_space, mj);

    if (mj->file->lexer_index >= mj->file->len || !mj->file->str)
        return NULL;

    char *start = mj->file->str + mj->file->lexer_index;

    skip_class(is_word, mj);

    char *end = mj->file->str + mj->file->lexer_index;

    if (end == start)
        return NULL;

    size_t len = end - start;
    char *word = strndup(start, len);

    return word;
}

char *get_operator(struct major *mj)
{
    if (mj->file->str && mj->file->str[mj->file->lexer_index] == ';')
        mj->file->lexer_index++;

    skip_class(my_is_space, mj);

    if (mj->file->lexer_index >= mj->file->len || !mj->file->str)
        return NULL;

    char *start = mj->file->str + mj->file->lexer_index;
    skip_class(is_operator, mj);

    char *end = mj->file->str + mj->file->lexer_index;

    if (end == start)
        return NULL;

    size_t len = end - start;
    char *word = strndup(start, len);

    return word;
}

char *get_redir(struct major *mj)
{
    if (mj->file->str && mj->file->str[mj->file->lexer_index] == ';')
        mj->file->lexer_index++;

    skip_class(my_is_space, mj);

    if (mj->file->lexer_index >= mj->file->len || !mj->file->str)
        return NULL;

    char *start = mj->file->str + mj->file->lexer_index;

    if (start && *start >= '0' && *start <= '9')
        mj->file->lexer_index++;

    skip_class(c_is_redir, mj);

    char *end = mj->file->str + mj->file->lexer_index;

    if (end == start)
        return NULL;

    size_t len = end - start;
    char *word = strndup(start, len);

    return word;
}

char *get_first_word(struct major *mj)
{
    char *word = NULL;
    do
    {
        size_t temp_index = mj->file->lexer_index;
        word = get_operator(mj);

        if (!word)
        {
            mj->file->lexer_index = temp_index;
            word = get_redir(mj);
        }

        if (!word)
        {
            mj->file->lexer_index = temp_index;
            word = get_word(mj);
        }

        if (word && *word == '#')
        {
            free(word);
            word = NULL;
            skip_to_new_line(mj);

            if (!mj->file->str || mj->file->lexer_index >= mj->file->len)
                break;
        }
        else if (!word)
        {
            mj->file->str = custom_fgets(mj->file->str, BUFFER_SIZE, mj->file);

            if (!mj->file->str)
                break;
        }
    } while (!word);

    return word;
}