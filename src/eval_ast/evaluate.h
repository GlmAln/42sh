#ifndef EVALUATE_H
#define EVALUATE_H

#include "../ast/ast.h"
#include "../builtins/builtins.h"
#include "../function/function.h"
#include "../hash/hash.h"
typedef int (*CommandFunction)(char **, size_t, struct hash **);

int execCommand(struct ast_command *t, struct hash **hash,
                struct function *func);
int executeScript(int j, char **argv);
int evaluate(struct ast *t, struct hash **hash, struct function *func);

#endif // ! EVALUATE_H
