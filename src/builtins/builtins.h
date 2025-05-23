#ifndef BUILTINS_H
#define BUILTINS_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../function/function.h"
#include "../hash/hash.h"
void echo(char **str, int num);
int True();
int False();

int builtins_cd(int argc, char **str, struct hash **hash_map);
int builtins_continue(void);
int builtins_break(void);
int builtins_exit(int argc, char **str);
int builtins_export(struct ast_command *t, struct hash **hash_map);
int builtins_dot(char *str, struct hash **hash_map, struct function *func);
int builtins_unset(int argc, char **argv, struct hash **hash_map,
                   struct function *func);

#endif // ! BUILTINS_H
