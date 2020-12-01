#ifndef REDIRECTION_H
#define REDIRECTION_H

#include "my_utils.h"

/**
 * @brief Init a struct redir
 *
 * @param mj
 * @return struct redir*
 */
struct redir *init_redirection(struct major *mj);

/**
 * @brief Set the redirection type depending the word and
 * the redirection file depending the file
 *
 * @param mj
 * @param redirection
 * @param word
 * @param file
 */
void set_redirection(struct major *mj, struct redir *redirection, char *word,
                     char *file);

/**
 * @brief Frees the structure redirection
 *
 * @param redir
 */
void free_redirection(struct redir *redir);

/**
 * @brief Check if the next word is a redir or not
 *
 * @param mj
 * @return int
 */
int next_is_redirection(struct major *mj);

#endif /* REDIRECTION_H */