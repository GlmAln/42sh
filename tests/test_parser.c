#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>

#include "../src/ast/ast.h"
#include "../src/function/function.h"
#include "../src/hash/hash.h"
#include "../src/lexer/lexer.h"
#include "../src/parser/parser.h"

int main(void)
{
    char *str = "{ true || false || true }";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;
    lexer_init(input, &lexer);
    struct ast *res = NULL;
    struct function *func = function_new(4);
    if (input_parse(&res, &lexer, func) != PARSER_OK)
    {
        lexer_free(&lexer);
        function_free(func);
        return 2;
    }
    struct hash *hash = hash_new(4);
    int exit_code = 0;
    ast_free(res, NULL);
    hash_free(hash);
    lexer_free(&lexer);
    return exit_code;
}
