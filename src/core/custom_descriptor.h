#ifndef CUSTOM_DESCRIPTOR_H
#define CUSTOM_DESCRIPTOR_H

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
    int index;
    char *str;
    size_t len;
};

#endif