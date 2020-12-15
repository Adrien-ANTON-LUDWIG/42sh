#ifndef CHAR_ARRAY_OP_
#define CHAR_ARRAY_OP_

#include "list.h"
#include "major.h"

/**
 * @brief Converts the command from struct list to a char **
 *
 * @param l
 * @return char**
 */
char **token_list_to_char_array(struct list *l);

/**
 * @brief Duplicates the argv char array
 *
 * @param mj
 * @param argv
 * @return char**
 */
char **char_array_dup(struct major *mj, char **argv);

/**
 * @brief Frees the argv char array
 *
 * @param argv
 */
void char_array_free(char **argv);

#endif /* CHAR_ARRAY_OP_ */