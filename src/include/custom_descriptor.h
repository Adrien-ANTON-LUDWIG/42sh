#ifndef CUSTOM_DESCRIPTOR_H
#define CUSTOM_DESCRIPTOR_H

#include <stddef.h>
#include <stdio.h>

#define CUSTOM_FD -5
/**
 * 42sh can be called with the -c option,
 * with a file as an argument or with stdin as the input.
 *
 * Here, a custom descriptor is a wrapper around the classical file I/O
 * operations This way, the code is the same for the rest of the program
 *
 * This means that the file MUST BE passed as a custom_FILE by the core to the
 * lexer ! No filename, FILE or fd involved
 */

/**
 * @brief if fd == CUSTOM_FD then we will be reading str instead of using fgets.
 * If fd != CUSTOM_FD index and str will not be used
 *
 */
struct custom_FILE
{
    int fd;
    FILE *file;
    size_t index;
    char *str;
    size_t len;
};

struct custom_FILE *custom_fopen(const char *path);
struct custom_FILE *createfrom_string(char *str);
void custom_fclose(struct custom_FILE *f);
char *custom_fgets(char *s, size_t size, struct custom_FILE *f);

#endif