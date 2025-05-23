#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "ast/ast.h"
#include "builtins/builtins.h"
#include "eval_ast/evaluate.h"
#include "function/function.h"
#include "hash/hash.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

struct hash *init_hash(size_t size)
{
    struct hash *hash = hash_new(size);
    char tmp[1024];
    hash_set(hash, "PWD", getcwd(tmp, 1024));
    hash_set(hash, "HOME", getcwd(tmp, 1024));
    hash_set(hash, "?", "0");
    hash_set(hash, "#", "0");
    snprintf(tmp, 1024, "%u", getuid());
    hash_set(hash, "UID", tmp);
    hash_set(hash, "IFS", " ");
    srand(time(NULL));
    snprintf(tmp, 1024, "%i", rand() % 99999);
    hash_set(hash, "RANDOM", "tmp");
    return hash;
}

static void set_args(struct hash *hash, char *argv[], int argc)

{
    char *value = calloc(1, sizeof(char));
    size_t size = 0;
    int i = 2;
    while (argv[i])
    {
        value = realloc(value, size + strlen(argv[i]) + 2);
        char str[1024];
        snprintf(str, 1024, "%i", i - 1);
        hash_set(hash, str, argv[i]);

        char *ptr1 = argv[i++];
        while (*ptr1)
            value[size++] = *ptr1++;
        value[size++] = ' ';
    }
    value[size] = 0;

    hash_set(hash, "@", value);
    hash_set(hash, "*", value);
    char size_str[1024];
    snprintf(size_str, 1024, "%i", argc - 2);
    hash_set(hash, "#", size_str);

    free(value);
}

static void eval_parse(struct lexer *lexer, struct hash *hash, int *exit_code)
{
    struct ast *res = NULL;
    struct function *func = function_new(4);
    int eof = 0;
    int res_parse;
    while (!eof && *exit_code >= 0
           && (res_parse = input_parse(&res, lexer, func, &eof)) == PARSER_OK)
    {
        *exit_code = evaluate(res, &hash, func);
        ast_free(res, NULL);
    }
    *exit_code = res_parse == PARSER_OK ? *exit_code : 2;
    function_free(func);
}

int main(int argc, char *argv[])
{
    int init_args = 0;
    FILE *input_stream = NULL;

    if (argc < 2)
        input_stream = stdin;
    else
    {
        if (*argv[1] != '-')
        {
            init_args = 1;
            input_stream = fopen(argv[1], "r");

            if (input_stream == NULL)
            {
                fprintf(stderr, "42sh: could not open file '%s'\n", argv[1]);
                return 1;
            }
        }
        else if (strcmp(argv[1], "-c") == 0)
            input_stream =
                fmemopen(argv[2], strlen(argv[2] ? argv[2] : ""), "r");
        else
        {
            fprintf(stderr, "42sh: unrecognized option '%s'\n", argv[1]);
            printf("Usage: 42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]\n");
            return 1;
        }
    }
    if (input_stream == NULL || fseek(input_stream, 0, SEEK_CUR) == -1)
        return 0;
    int exit_code = 0;
    struct lexer lexer;
    if ((exit_code = lexer_init(input_stream, &lexer)))
        return exit_code;
    struct hash *hash = init_hash(4);
    if (init_args)
        set_args(hash, argv, argc);

    eval_parse(&lexer, hash, &exit_code);

    hash_free(hash);
    lexer_free(&lexer);
    fclose(input_stream);

    return exit_code < -512 ? exit_code + 1024 : exit_code;
}
