#define _POSIX_C_SOURCE 200809L
#include "lexer_utils.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"

static int my_is_space(int c)
{
    return c == ' ' || c == '\t' || c == '\v';
}

/**
 * @brief Returns 1 if a given char is not a space of separator. Else return 0;
 *
 * @param c
 * @return int
 */
static int is_word(int c)
{
    return c != '\0' && !my_is_space(c) && c != ';' && c != '\n' && c != '\r';
}

/**
 * @brief
 *
 * @param classifier
 * @param cursor
 */
void skip_class(int (*classifier)(int), struct major *mj)
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

/**
 * @brief Get the word object
 *
 * @param s
 * @return char*
 */
char *get_word(struct major *mj)
{
    if (mj->file->str && mj->file->str[mj->file->lexer_index] == ';')
        mj->file->lexer_index++;
    // Prendre en charge le buffer plein (>512)
    skip_class(my_is_space, mj);

    if (mj->file->lexer_index >= mj->file->len - 1 || !mj->file->str)
        return NULL;

    char *start = mj->file->str + mj->file->lexer_index;
    skip_class(is_word, mj);

    char *end = mj->file->str + mj->file->lexer_index;

    if (end == start)
        return NULL;

    size_t len = end - start;
    char *word = strndup(start, len);

    skip_class(my_is_space, mj);

    return word;
}