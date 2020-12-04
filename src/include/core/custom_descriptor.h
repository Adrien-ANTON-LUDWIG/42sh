#ifndef CUSTOM_DESCRIPTOR_H
#define CUSTOM_DESCRIPTOR_H

#include <stddef.h>
#include <stdio.h>

#include "structures.h"

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
    size_t lexer_index;
    char *str;
    size_t len;
};

/**
 * @brief Simulates a FILE structure if path is NULL
 *
 * @param path Can be NULL
 * @return struct custom_FILE*
 */
struct custom_FILE *custom_fopen(const char *path);

/**
 * @brief Creates a custom_FILE structure from a string
 *
 * @param str
 * @return struct custom_FILE*
 */
struct custom_FILE *createfrom_string(char *str);

/**
 * @brief If the custom_FILE is linked to a FILE structure
 * the FILE is closed
 * Frees the allocated custom_FILE
 *
 * @param f
 */
void custom_fclose(struct custom_FILE *f);

/**
 * @brief Same as fgets but works with strings
 *
 * @param s buffer
 * @param size amount of bytes to read
 * @param f
 * @return char* pointer to s
 */
char *custom_fgets(char *s, size_t size, struct custom_FILE *f);

/**
 * @brief Takes a custom file and returns a single
 * string whith all its data.
 *
 * @param f custom_FILE
 * @return char*
 */
// char *custom_getfile(struct custom_FILE *f);
void get_new_string(struct major *mj);

#endif