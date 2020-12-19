#ifndef SHOPT_H_
#define SHOPT_H_

#include "major.h"

/**
 * @brief Manages -O and +O
 *
 * @param mj
 * @param argv
 * @return int
 */
int shopt_options_argv(struct major *mj, char **argv);

/**
 * @brief Inits the list of shopt options
 *
 * @param mj
 */
void shopt_init_list(struct major *mj);

/**
 * @brief Frees the list of shopt options
 *
 * @param mj
 */
void shopt_free_list(struct major *mj);

/**
 * @brief Set the shopt opption 'name' to value
 *
 * @param sol
 * @param opt_name
 * @param value
 */
int shopt_set_opt(struct major *mj, char *opt_name, int value);

/**
 * @brief Checks if the shopt 'opt_name' is set or not
 *
 * @param mj
 * @param opt_name
 * @return int
 */
int shopt_opt_is_set(struct major *mj, char *opt_name);

/**
 * @brief Called for the builtin 'shopt'
 *
 * @param mj
 * @param argv
 * @return int
 */
int b_shopt_options(struct major *mj, char **argv);

#endif /* SHOPT_H_ */