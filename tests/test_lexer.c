#define _POSIX_C_SOURCE 200809L

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/lexer/lexer.h"
/*
Test(Test_lexer, lexer_peek_pop_vide)
{
    char *str = "\0";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1,sizeof(char **));

    enum token_type type = lexer_peek(&lexer,result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_eq(*result,NULL);

    type = lexer_pop(&lexer,result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_eq(*result,NULL);

    type = lexer_peek(&lexer,result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_eq(*result,NULL);

    type = lexer_pop(&lexer,result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_eq(*result,NULL);

    type = lexer_peek(&lexer,result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_eq(*result,NULL);

    type = lexer_pop(&lexer,result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_eq(*result,NULL);

    lexer_free(&lexer);
}

Test(Test_lexer, lexer_vide)
{
    char input[] = {0,-1};
    struct lexer *lexer =malloc(sizeof(struct lexer));
    //lexer_init(input,lexer);            //tmp
    lexer->input = input;
    lexer->position = input;
    char **result = calloc(1,sizeof(char **));

    enum token_type type = lexer_peek(lexer,result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_eq(*result,NULL);

    type = lexer_pop(lexer,result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_eq(*result,NULL);

    //lexer_free(lexer); // tmp
    free(lexer);
    free(result);
}

Test(Test_lexer, lexer_plusieurs_vide)
{
    char input[] = {0,0,0,0,0,-1};
    struct lexer *lexer =malloc(sizeof(struct lexer));
    //lexer_init(input,lexer);            //tmp
    lexer->input = input;
    lexer->position = input;
    char **result = calloc(1,sizeof(char **));

    enum token_type type = lexer_peek(lexer,result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_eq(*result,NULL);

    type = lexer_pop(lexer,result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_eq(*result,NULL);

    //lexer_free(lexer); // tmp
    free(lexer);
    free(result);
}*/

Test(Test_lexer, lexer_word)
{
    char *str = "test";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "test");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "test");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "test");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "test");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_negation)
{
    char *str = "!";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_NEGATION);
    cr_assert_str_eq(*result, "!");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_NEGATION);
    cr_assert_str_eq(*result, "!");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "!");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "!");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_leftpara)
{
    char *str = "(";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_LEFTPARA);
    cr_assert_str_eq(*result, "(");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_LEFTPARA);
    cr_assert_str_eq(*result, "(");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "(");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "(");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_rightpara)
{
    char *str = ")";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_RIGHTPARA);
    cr_assert_str_eq(*result, ")");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_RIGHTPARA);
    cr_assert_str_eq(*result, ")");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, ")");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, ")");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_leftaco)
{
    char *str = "{";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_LEFTACO);
    cr_assert_str_eq(*result, "{");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_LEFTACO);
    cr_assert_str_eq(*result, "{");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "{");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "{");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_rightaco)
{
    char *str = "}";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_RIGHTACO);
    cr_assert_str_eq(*result, "}");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_RIGHTACO);
    cr_assert_str_eq(*result, "}");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "}");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "}");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_case)
{
    char *str = "case";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_CASE);
    cr_assert_str_eq(*result, "case");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_CASE);
    cr_assert_str_eq(*result, "case");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "case");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "case");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_do)
{
    char *str = "do";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_DO);
    cr_assert_str_eq(*result, "do");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_DO);
    cr_assert_str_eq(*result, "do");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "do");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "do");

    lexer_free(&lexer);
    free(result);
}
Test(Test_lexer, lexer_done)
{
    char *str = "done";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_DONE);
    cr_assert_str_eq(*result, "done");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_DONE);
    cr_assert_str_eq(*result, "done");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "done");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "done");

    lexer_free(&lexer);
    free(result);
}
Test(Test_lexer, lexer_elif)
{
    char *str = "elif";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_ELIF);
    cr_assert_str_eq(*result, "elif");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_ELIF);
    cr_assert_str_eq(*result, "elif");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "elif");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "elif");

    lexer_free(&lexer);
    free(result);
}
Test(Test_lexer, lexer_else)
{
    char *str = "else";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_ELSE);
    cr_assert_str_eq(*result, "else");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_ELSE);
    cr_assert_str_eq(*result, "else");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "else");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "else");

    lexer_free(&lexer);
    free(result);
}
Test(Test_lexer, lexer_esac)
{
    char *str = "esac";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_ESAC);
    cr_assert_str_eq(*result, "esac");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_ESAC);
    cr_assert_str_eq(*result, "esac");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "esac");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "esac");

    lexer_free(&lexer);
    free(result);
}
Test(Test_lexer, lexer_fi)
{
    char *str = "fi";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_FI);
    cr_assert_str_eq(*result, "fi");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_FI);
    cr_assert_str_eq(*result, "fi");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "fi");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "fi");

    lexer_free(&lexer);
    free(result);
}
Test(Test_lexer, lexer_for)
{
    char *str = "for";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_FOR);
    cr_assert_str_eq(*result, "for");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_FOR);
    cr_assert_str_eq(*result, "for");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "for");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "for");

    lexer_free(&lexer);
    free(result);
}
Test(Test_lexer, lexer_if)
{
    char *str = "if";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_IF);
    cr_assert_str_eq(*result, "if");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_IF);
    cr_assert_str_eq(*result, "if");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "if");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "if");

    lexer_free(&lexer);
    free(result);
}
Test(Test_lexer, lexer_in)
{
    char *str = "in";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_IN);
    cr_assert_str_eq(*result, "in");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_IN);
    cr_assert_str_eq(*result, "in");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "in");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "in");

    lexer_free(&lexer);
    free(result);
}
Test(Test_lexer, lexer_then)
{
    char *str = "then";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_THEN);
    cr_assert_str_eq(*result, "then");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_THEN);
    cr_assert_str_eq(*result, "then");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "then");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "then");

    lexer_free(&lexer);
    free(result);
}
Test(Test_lexer, lexer_until)
{
    char *str = "until";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_UNTIL);
    cr_assert_str_eq(*result, "until");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_UNTIL);
    cr_assert_str_eq(*result, "until");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "until");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "until");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_while)
{
    char *str = "while";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_WHILE);
    cr_assert_str_eq(*result, "while");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_WHILE);
    cr_assert_str_eq(*result, "while");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "while");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "while");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_semicolon)
{
    char *str = ";";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_SEMICOLON);
    cr_assert_str_eq(*result, ";");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_SEMICOLON);
    cr_assert_str_eq(*result, ";");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, ";");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, ";");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_redi1r)
{
    char *str = ">";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_REDIRECTION_1R);
    cr_assert_str_eq(*result, ">");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_REDIRECTION_1R);
    cr_assert_str_eq(*result, ">");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, ">");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, ">");

    lexer_free(&lexer);
    free(result);
}
Test(Test_lexer, lexer_redi2r)
{
    char *str = ">>";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_REDIRECTION_2R);
    cr_assert_str_eq(*result, ">>");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_REDIRECTION_2R);
    cr_assert_str_eq(*result, ">>");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, ">>");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, ">>");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_redi1l)
{
    char *str = "<";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_REDIRECTION_1L);
    cr_assert_str_eq(*result, "<");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_REDIRECTION_1L);
    cr_assert_str_eq(*result, "<");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "<");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "<");

    lexer_free(&lexer);
    free(result);
}
Test(Test_lexer, lexer_redi2l)
{
    char *str = "<<";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_REDIRECTION_2L);
    cr_assert_str_eq(*result, "<<");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_REDIRECTION_2L);
    cr_assert_str_eq(*result, "<<");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "<<");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "<<");

    lexer_free(&lexer);
    free(result);
}
Test(Test_lexer, lexer_redilf)
{
    char *str = "<>";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_REDIRECTION_LF);
    cr_assert_str_eq(*result, "<>");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_REDIRECTION_LF);
    cr_assert_str_eq(*result, "<>");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "<>");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "<>");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_1AND)
{
    char *str = "&";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_1AND);
    cr_assert_str_eq(*result, "&");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_1AND);
    cr_assert_str_eq(*result, "&");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "&");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "&");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_2AND)
{
    char *str = "&&";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_2AND);
    cr_assert_str_eq(*result, "&&");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_2AND);
    cr_assert_str_eq(*result, "&&");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "&&");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "&&");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_redi1pipe)
{
    char *str = "|";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_1PIPE);
    cr_assert_str_eq(*result, "|");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_1PIPE);
    cr_assert_str_eq(*result, "|");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "|");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "|");

    lexer_free(&lexer);
    free(result);
}
Test(Test_lexer, lexer_2pipe)
{
    char *str = "||";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_2PIPE);
    cr_assert_str_eq(*result, "||");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_2PIPE);
    cr_assert_str_eq(*result, "||");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "||");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "||");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_ionumber)
{
    char *str = "1 >";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_IONUMBER);
    cr_assert_str_eq(*result, "1");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_IONUMBER);
    cr_assert_str_eq(*result, "1");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_REDIRECTION_1R);
    cr_assert_str_eq(*result, ">");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_REDIRECTION_1R);
    cr_assert_str_eq(*result, ">");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, ">");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, ">");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_exaustif)
{
    char *str = "if true;then echo ok; else echo nop;fi";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_IF);
    cr_assert_str_eq(*result, "if");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_IF);
    cr_assert_str_eq(*result, "if");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "true");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "true");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_SEMICOLON);
    cr_assert_str_eq(*result, ";");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_SEMICOLON);
    cr_assert_str_eq(*result, ";");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_THEN);
    cr_assert_str_eq(*result, "then");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_THEN);
    cr_assert_str_eq(*result, "then");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "echo");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "echo");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "ok");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "ok");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_SEMICOLON);
    cr_assert_str_eq(*result, ";");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_SEMICOLON);
    cr_assert_str_eq(*result, ";");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_ELSE);
    cr_assert_str_eq(*result, "else");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_ELSE);
    cr_assert_str_eq(*result, "else");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "echo");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "echo");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "nop");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "nop");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_SEMICOLON);
    cr_assert_str_eq(*result, ";");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_SEMICOLON);
    cr_assert_str_eq(*result, ";");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_FI);
    cr_assert_str_eq(*result, "fi");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_FI);
    cr_assert_str_eq(*result, "fi");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "fi");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_EOF);
    cr_assert_str_eq(*result, "fi");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_exaustif2)
{
    char *str = "echo 'test ok                    &'";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "echo");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "echo");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "test ok                    &");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "test ok                    &");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_exaustif3)
{
    char *str = "echo 'test ok        ////////'";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "echo");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "echo");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "test ok        ////////");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "test ok        ////////");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_exaustif4)
{
    char *str = "echo \"test ok    \n    ////////\"";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "echo");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "echo");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "test ok    \n    ////////");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "test ok    \n    ////////");

    lexer_free(&lexer);
    free(result);
}

Test(Test_lexer, lexer_exaustif5)
{
    char *str = "echo \"test ok    \n    ////////\"";
    FILE *input = fmemopen(str, strlen(str), "r");
    struct lexer lexer;

    lexer_init(input, &lexer);
    char **result = calloc(1, sizeof(char **));

    enum token_type type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "echo");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "echo");

    type = lexer_peek(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "test ok    \n    ////////");

    type = lexer_pop(&lexer, result);
    cr_assert_eq(type, TOKEN_WORDS);
    cr_assert_str_eq(*result, "test ok    \n    ////////");

    lexer_free(&lexer);
    free(result);
}
