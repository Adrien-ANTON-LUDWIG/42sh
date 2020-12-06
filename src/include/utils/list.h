#ifndef LIST_H_
#define LIST_H_

#include <stddef.h>

#include "major.h"

struct list
{
    struct list_item *head;
    struct list_item *tail;
    size_t size;
};

struct list_item
{
    struct list_item *next;
    char *name;
    char *data;
};

/**
 * @brief Appends a string str to the list list
 *
 * @param mj
 * @param list
 * @param str
 * @return struct list*
 */
struct list *list_append(struct major *mj, struct list *list, char *str);

/**
 * @brief Appends a variable with name and data to the list list
 *
 * @param mj
 * @param list
 * @param name
 * @param str
 * @return struct list*
 */
struct list *list_append_variable(struct major *mj, struct list *list,
                                  char *name, char *str);

/**
 * @brief Frees the list l
 *
 * @param l
 */
void list_free(struct list *l);

/**
 * @brief Inits a struct list
 *
 * @param mj
 * @return struct list*
 */
struct list *list_init(struct major *mj);

#endif /* LIST_H_ */