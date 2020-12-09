#ifndef CUSTOM_DESCRIPTOR_H
#define CUSTOM_DESCRIPTOR_H

#include <stddef.h>
#include <stdio.h>

#include "major.h"

#define CUSTOM_FD -5
#define BUFFER_SIZE 512
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
    size_t buffer_size;
};

/**
 * @brief Simulates a FILE structure if path is NULL
 *
 * @param path Can be NULL
 * @return struct custom_FILE*
 */
struct custom_FILE *custom_fopen(struct major *mj, const char *path);

/**
 * @brief Creates a custom_FILE structure from a string
 *
 * @param str
 * @return struct custom_FILE*
 */
struct custom_FILE *createfrom_string(struct major *mj, char *str);

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

/**
 * @brief Same as getline but works with a custom_FILE.
 *
 *
 * This function should be called when the buffer mj->file->str was entirely
 * read.
 * This function overrides the buffer.
 * Therefore, this function should only be called when everything was read
 * if the custom_FILE was created with a string. That's why it always returns 0
 * in this case.
 *
 * @param mj Contains mj->file which is the custom_FILE to update.
 * @return returns 1 upon succes, 0 otherwise
 */
int custom_getline(struct major *mj);

/**
 * @brief This function works as custom_getline() but it does not override the
 * buffer, it expands it.
 *
 * This function should be called when the buffer mj->file->str was entirely
 * read.
 * Therefore, this function should only be called when everything was read
 * if the custom_FILE was created with a string. That's why it always returns 0
 * in this case.
 *
 * @param mj Contains mj->file which is the custom_FILE to update.
 * @return returns 1 upon succes, 0 otherwise
 */
int custom_getline_same_buf(struct major *mj);

#endif