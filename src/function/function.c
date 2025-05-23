#include "function.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t function(char *key)
{
    size_t i = 0;
    size_t function = 0;

    for (i = 0; key[i] != 0; i++)
        function += key[i];
    function += i;

    return function;
}

static void free_pair_fun_list(struct pair_fun *pair_fun)
{
    if (pair_fun == NULL)
        return;
    else
    {
        free_pair_fun_list(pair_fun->next);
        free(pair_fun->key);
        ast_free(pair_fun->value, NULL);
        free(pair_fun);
    }
}
int function_is_empty(struct function *func)
{
    for (size_t i = 0; i < func->size; i++)
    {
        if (func->data[i] != NULL)
            return 0;
    }
    return 1;
}

struct function *function_new(size_t size)
{
    struct function *function_map = malloc(sizeof(struct function));

    if (function_map == NULL)
        return NULL;

    function_map->data = calloc(size, sizeof(struct pair_fun *));
    function_map->size = size;

    if (function_map->data == NULL)
    {
        free(function_map);
        return NULL;
    }

    return function_map;
}

int function_set(struct function *function_map, char *key, struct ast *value)
{
    size_t index = function(key);
    if (index > function_map->size)
        index = index % function_map->size;

    struct pair_fun *pair_fun = function_map->data[index];
    struct pair_fun *prev = pair_fun;
    while (pair_fun != NULL && strcmp(pair_fun->key, key))
    {
        prev = pair_fun;
        pair_fun = pair_fun->next;
    }

    if (pair_fun != NULL)
    {
        if (value != NULL)
        {
            ast_free(pair_fun->value, NULL);
            pair_fun->value = value;
        }
        else
        {
            if (prev == pair_fun)
                function_map->data[index] = pair_fun->next;
            else
                prev->next = pair_fun->next;
            free(pair_fun->key);
            ast_free(pair_fun->value, NULL);
            free(pair_fun);
        }
    }
    else if (value != NULL)
    {
        struct pair_fun *new = malloc(sizeof(struct pair_fun));

        if (new == NULL)
            return 0;

        char *new_key = malloc(strlen(key) + 1);
        new->key = memcpy(new_key, key, strlen(key) + 1);
        new->next = function_map->data[index];
        new->value = value;
        function_map->data[index] = new;
    }

    return 1;
}

struct ast *function_get(struct function *function_map, char *key)
{
    size_t index = function(key);
    if (index > function_map->size)
        index = index % function_map->size;

    struct pair_fun *pair_fun = function_map->data[index];
    while (pair_fun != NULL && strcmp(pair_fun->key, key))
        pair_fun = pair_fun->next;

    if (pair_fun == NULL)
        return NULL;
    else
        return pair_fun->value;
}

void function_free(struct function *function_map)
{
    for (size_t i = 0; i < function_map->size; i++)
        free_pair_fun_list(function_map->data[i]);
    free(function_map->data);
    free(function_map);
}
