#ifndef STRUCTURES_H
#define STRUCTURES_H

struct major
{
    int options;
    struct custom_FILE *file;
    struct list *variables;
};

#endif /* STRUCTURES_H */