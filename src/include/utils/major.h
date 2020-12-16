#ifndef MAJOR_H
#define MAJOR_H

#include <stddef.h>

#define MIN(A, B) A < B ? A : B
#define BREAK_AST -42

/**
 * @brief Stores the function declared inside this shell
 * for later use
 *
 */
struct funclist
{
    struct funclist *next;
    struct ast *ast;
};

/**
 * @brief Shopt options list
 *
 */
struct shopt_opt_list
{
    struct shopt_opt_list *next;
    char *name;
    int value;
};
/**
 * @brief Contains all the declared variables and their value
 *
 */
struct varlist
{
    struct varlist *next;
    char *name;
    char *value;
};

/**
 * @brief Majors stores the important structures we use all along the project.
 * It also allows us to exit and free those structures whenever we need it
 * during the execution.
 *
 */
struct major
{
    int rvalue;
    int options;
    int loop_counter;
    int break_counter;
    size_t continue_counter;
    struct custom_FILE *file;
    struct varlist *variables;
    struct funclist *flist;
    struct shopt_opt_list *shopt_opt;
    char **arguments;
};

/**
 * @brief Initializes major
 *
 * @return struct major*
 */
struct major *major_init(void);

/**
 * @brief Frees struct major
 *
 * @param mj
 */
void major_free(struct major *mj);

/**
 * @brief Add a function ast to the list of functions
 *
 * @param mj
 * @param func
 */
int add_to_funclist(struct major *mj, struct ast *func);

#endif /* MAJOR_H_ */