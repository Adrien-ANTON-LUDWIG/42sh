#ifndef MAJOR_H_
#define MAJOR_H_

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
    struct custom_FILE *file;
    struct list *variables;
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

#endif /* MAJOR_H_ */