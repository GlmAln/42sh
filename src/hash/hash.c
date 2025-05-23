#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static size_t hash(char *key)
{
    size_t i = 0;
    size_t hash = 0;

    for (i = 0; key[i] != 0; i++)
        hash += key[i];
    hash += i;

    return hash;
}

static void free_pair_list(struct pair *pair)
{
    if (pair == NULL)
        return;
    else
    {
        free_pair_list(pair->next);
        free(pair->key);
        free(pair->value);
        free(pair);
    }
}

static void print_pair(struct pair *pair)
{
    if (pair == NULL)
        return;
    else
    {
        printf(", %s: %s", pair->key, pair->value);
        print_pair(pair->next);
    }
}

struct hash *hash_new(size_t size)
{
    struct hash *hash_map = malloc(sizeof(struct hash));

    if (hash_map == NULL)
        return NULL;

    hash_map->data = calloc(size, sizeof(struct pair *));
    hash_map->size = size;

    if (hash_map->data == NULL)
    {
        free(hash_map);
        return NULL;
    }

    return hash_map;
}

int hash_set(struct hash *hash_map, char *key, char *value)
{
    size_t index = hash(key);
    if (index > hash_map->size)
        index = index % hash_map->size;

    struct pair *pair = hash_map->data[index];
    struct pair *prev = pair;
    while (pair != NULL && strcmp(pair->key, key))
    {
        prev = pair;
        pair = pair->next;
    }

    if (pair != NULL)
    {
        if (value != NULL)
        {
            free(pair->value);
            char *tmp = calloc(1, strlen(value) + 1);
            strcpy(tmp, value);
            pair->value = tmp;
        }
        else
        {
            if (prev == pair)
                hash_map->data[index] = pair->next;
            else
                prev->next = pair->next;
            free(pair->key);
            free(pair->value);
            free(pair);
        }
    }
    else if (value != NULL)
    {
        struct pair *new = malloc(sizeof(struct pair));

        if (new == NULL)
            return 0;

        char *new_key = malloc(strlen(key) + 1);
        char *new_value = malloc(strlen(value) + 1);
        new->key = memcpy(new_key, key, strlen(key) + 1);
        new->value = memcpy(new_value, value, strlen(value) + 1);
        new->next = hash_map->data[index];
        hash_map->data[index] = new;
    }

    return 1;
}

char *hash_get(struct hash *hash_map, char *key)
{
    char str[1024];
    srand(time(NULL));
    snprintf(str, 1024, "%i", rand() % 99999);
    hash_set(hash_map, "RANDOM", str);
    size_t index = hash(key);
    if (index > hash_map->size)
        index = index % hash_map->size;

    struct pair *pair = hash_map->data[index];
    while (pair != NULL && strcmp(pair->key, key))
        pair = pair->next;

    if (pair == NULL)
        return NULL;
    else
        return pair->value;
}

void hash_free(struct hash *hash_map)
{
    for (size_t i = 0; i < hash_map->size; i++)
        free_pair_list(hash_map->data[i]);
    free(hash_map->data);
    free(hash_map);
}

void hash_print(struct hash *hash_map)
{
    for (size_t i = 0; i < hash_map->size; i++)
    {
        if (hash_map->data[i] != NULL)
        {
            struct pair *pair = hash_map->data[i];
            printf("%s: %s", pair->key, pair->value);
            print_pair(pair->next);
            putchar('\n');
        }
    }
}

/*
int main(void)
{
    struct hash *hash_map = hash_new(2);

    hash_set(hash_map, "key1", "value1");
    hash_set(hash_map, "key2", "value2");
    hash_set(hash_map, "key3", "value3");
    hash_print(hash_map);

    hash_set(hash_map, "key4", NULL);
    hash_print(hash_map);

    hash_set(hash_map, "key2", NULL);
    hash_print(hash_map);

    hash_free(hash_map);
    return 0;
}
*/
