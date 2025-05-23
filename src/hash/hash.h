#ifndef HASH_H
#define HASH_H

#include <stddef.h>

struct pair
{
    char *key;
    char *value;
    struct pair *next;
};

struct hash
{
    struct pair **data;
    size_t size;
};

struct hash *hash_new(size_t size);

int hash_set(struct hash *hash_map, char *key, char *value);
char *hash_get(struct hash *hash_map, char *key);

void hash_free(struct hash *hash_map);
void hash_print(struct hash *hash_map);

#endif /* !HASH_H */
