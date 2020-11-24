#include "custom_descriptor.h"

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Simulates a FILE structure if path is NULL
 *
 * @param path Can be NULL
 * @return struct custom_FILE*
 */
struct custom_FILE *custom_fopen(char *path)
{
    struct custom_FILE *f = malloc(sizeof(struct custom_FILE));
    if (!f) // The program should not have allocated anything at that point
        errx(2, "Could not allocate memory !");
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

/**
 * @brief Creates a custom_FILE structure from a string
 *
 * @param str
 * @return struct custom_FILE*
 */
struct custom_FILE *createfrom_string(char *str)
{
    struct custom_FILE *f = custom_fopen(NULL);
    f->str = str;
    f->file = NULL;
    f->len = strlen(str);
    return f;
}

/**
 * @brief If the custom_FILE is linked to a FILE structure
 * the FILE is closed
 * Frees the allocated custom_FILE
 *
 * @param f
 */
void custom_fclose(struct custom_FILE *f)
{
    if (f->fd == CUSTOM_FD)
        free(f->str);
    else
        fclose(f->file);
    free(f);
}

/**
 * @brief Same as fgets but works with strings
 *
 * @param s buffer
 * @param size amount of bytes to read
 * @param f
 * @return int amount of data read
 */
int custom_fgets(char *s, int size, struct custom_FILE *f)
{
    if (f->fd == CUSTOM_FD)
    {
        if (f->index == f->len)
            return EOF;
        strncpy(s, f->str + f->index, size);
        if (f->index + size >= f->len)
        {
            int count = f->len - f->index;
            f->index = f->len;
            return count;
        }
        f->index += size;
        return size;
    }
    return fgets(s, size, f->file);
}