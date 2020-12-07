#ifndef REDIR_H_
#define REDIR_H_

#include "ast.h"
#include "major.h"

/**
 * @brief Execute an ast which root is a WORD_REDIR
 *
 * @param mj
 * @param ast
 * @return int
 */
int exec_redir(struct major *mj, struct ast *ast);

#endif /* REDIR_H_ */