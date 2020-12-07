#define _POSIX_C_SOURCE 200809L
#include "custom_descriptor.h"

#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "my_xmalloc.h"

struct custom_FILE *custom_fopen(struct major *mj, const char *path)
{
    struct custom_FILE *f = my_xmalloc(mj, sizeof(struct custom_FILE));

    f->lexer_index = 0;
    f->buffer_size = 0;
    if (!path)
    {
        f->fd = CUSTOM_FD;
        f->index = 0;
        f->str = NULL;
        return f;
    }
    FILE *file = fopen(path, "r");
    if (!file)
    {
        free(f);
        errx(127, "Cannot open file: %s", path);
    }
    f->fd = file->_fileno;
    f->file = file;
    f->str = NULL;
    return f;
}

struct custom_FILE *createfrom_string(struct major *mj, char *str)
{
    struct custom_FILE *f = custom_fopen(mj, NULL);
    f->str = strdup(str);
    f->file = NULL;
    f->len = strlen(str);
    return f;
}

void custom_fclose(struct custom_FILE *f)
{
    free(f->str);
    if (f->fd != CUSTOM_FD)
        fclose(f->file);
    free(f);
}

char *custom_fgets(char *s, size_t size, struct custom_FILE *f)
{
    if (f->fd != CUSTOM_FD)
        return fgets(s, size, f->file);

    if (f->index >= f->len)
        return NULL;

    size_t i = 0;

    for (i = 0; i < size && f->index < f->len; i++)
    {
        s[i] = f->str[f->index];
        if (f->str[f->index] == '\n' || f->str[f->index] == 0)
        {
            f->index += 1;
            s[i + 1] = 0;
            return s;
        }
        f->index += 1;
    }
    s[i] = 0;
    return s;
}

void get_new_string(struct major *mj)
{
    if (!mj || mj->file->fd == CUSTOM_FD)
        return;

    mj->file->str = custom_fgets(mj->file->str, BUFFER_SIZE, mj->file);
    mj->file->lexer_index = 0;
    if (mj->file->str)
        mj->file->len = strlen(mj->file->str);
    else
        mj->file->len = 0;
}

int custom_getline(struct major *mj)
{
    struct custom_FILE *f = mj->file;

    if (f->fd == CUSTOM_FD)
        return 0;

    ssize_t ret = getline(&(f->str), &(f->buffer_size), f->file);

    if (ret == -1)
        return 0;

    f->len = ret;
    f->lexer_index = 0;

    return 1;
}

int custom_getline_same_buf(struct major *mj)
{
    struct custom_FILE *f = mj->file;

    if (f->fd == CUSTOM_FD)
        return 0;

    char *buffer = NULL;
    size_t size = 0;

    ssize_t ret = getline(&buffer, &size, f->file);

    if (ret == -1)
    {
        free(buffer);
        return 0;
    }

    if (f->lexer_index + ret >= f->buffer_size)
    {
        f->buffer_size = f->lexer_index + ret + 1;
        f->str = my_xrealloc(mj, f->str, f->buffer_size);
    }

    strcpy(f->str + f->lexer_index, buffer);
    f->len = f->buffer_size - 1;

    free(buffer);

    return 1;
}
