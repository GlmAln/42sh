#ifndef FUNCTION_H
#define FUNCTION_H

#include <stddef.h>

#include "../ast/ast.h"

struct pair_fun
{
    char *key;
    struct ast *value;
    struct pair_fun *next;
};

struct function
{
    struct pair_fun **data;
    size_t size;
};

struct function *function_new(size_t size);

int function_set(struct function *list, char *name, struct ast *value);
struct ast *function_get(struct function *list, char *name);

int function_is_empty(struct function *func);
void function_free(struct function *list);

#endif // ! FUNCTION_H
