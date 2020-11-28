#ifndef ARGUMENT_HANDLER_H
#define ARGUMENT_HANDLER_H

/**
 * @brief Get the index of the first non command string
 *
 * @param index the index after "-c"
 * @param argc
 * @param argv
 * @return int
 */
int get_index_command_string(int index, int argc, char *argv[]);

/**
 * @brief Merges argv into a single string
 *
 * @param argc
 * @param argv
 * @return char*
 */
char *merge_arguments(int argc, char **argv);

#endif
