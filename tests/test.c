#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/ast/ast.h"
#include "../src/eval_ast/evaluate.h"
#include "../src/lexer/lexer.h"
#include "../src/parser/parser.h"

/*========================TEST LEXER==========================================*/
/*
Test(Test_lexer, lexer_void)
{
    char *input = "";
    struct lexer *lexer = lexer_new(input);

    struct token token1 = lexer_peek(lexer);
    struct token token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_EOF);
    cr_assert_eq(token2.type, TOKEN_EOF);

    lexer_free(lexer);
}

Test(Test_lexer, lexer_void_2)
{
    char *input = "                     ";
    struct lexer *lexer = lexer_new(input);

    struct token token1 = lexer_peek(lexer);
    struct token token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_EOF);
    cr_assert_eq(token2.type, TOKEN_EOF);

    lexer_free(lexer);
}

Test(Test_lexer, lexer_ticky_if_echo_if)
{
    char *input = "   if  echo  if if  ;";
    struct lexer *lexer = lexer_new(input);

    struct token token1 = lexer_peek(lexer);
    struct token token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_IF);
    cr_assert_eq(token2.type, TOKEN_IF);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);
    cr_assert_str_eq(token1.value, "echo");

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);
    cr_assert_str_eq(token1.value, "if");

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);
    cr_assert_str_eq(token1.value, "if");

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_SEMICOLON);
    cr_assert_eq(token2.type, TOKEN_SEMICOLON);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_EOF);
    cr_assert_eq(token2.type, TOKEN_EOF);

    lexer_free(lexer);
}

Test(Test_lexer, lexer_simple_newline)
{
    char *input = "echo bonjour ttlm\\n";
    struct lexer *lexer = lexer_new(input);

    struct token token1 = lexer_peek(lexer);
    struct token token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_EOF);
    cr_assert_eq(token2.type, TOKEN_EOF);

    lexer_free(lexer);
}

Test(Test_lexer, lexer_complete_basic_if_elif_else_newlines)
{
    char *input =
        "if true;\nthen echo ok;\nelif false;\necho test;\nelse\n false;\nfi";
    struct lexer *lexer = lexer_new(input);

    struct token token1 = lexer_peek(lexer);
    struct token token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_IF);
    cr_assert_eq(token2.type, TOKEN_IF);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);
    cr_assert_str_eq(token1.value, "true");

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_SEMICOLON);
    cr_assert_eq(token2.type, TOKEN_SEMICOLON);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_NEWLINE);
    cr_assert_eq(token2.type, TOKEN_NEWLINE);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_THEN);
    cr_assert_eq(token2.type, TOKEN_THEN);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);
    cr_assert_str_eq(token1.value, "echo");

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);
    cr_assert_str_eq(token1.value, "ok");

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_SEMICOLON);
    cr_assert_eq(token2.type, TOKEN_SEMICOLON);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_NEWLINE);
    cr_assert_eq(token2.type, TOKEN_NEWLINE);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_ELIF);
    cr_assert_eq(token2.type, TOKEN_ELIF);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);
    cr_assert_str_eq(token1.value, "false");

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_SEMICOLON);
    cr_assert_eq(token2.type, TOKEN_SEMICOLON);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_NEWLINE);
    cr_assert_eq(token2.type, TOKEN_NEWLINE);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);
    cr_assert_str_eq(token1.value, "echo");

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);
    cr_assert_str_eq(token1.value, "test");

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_SEMICOLON);
    cr_assert_eq(token2.type, TOKEN_SEMICOLON);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_NEWLINE);
    cr_assert_eq(token2.type, TOKEN_NEWLINE);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_ELSE);
    cr_assert_eq(token2.type, TOKEN_ELSE);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_NEWLINE);
    cr_assert_eq(token2.type, TOKEN_NEWLINE);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);
    cr_assert_str_eq(token1.value, "false");

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_SEMICOLON);
    cr_assert_eq(token2.type, TOKEN_SEMICOLON);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_NEWLINE);
    cr_assert_eq(token2.type, TOKEN_NEWLINE);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_FI);
    cr_assert_eq(token2.type, TOKEN_FI);

    lexer_free(lexer);
}

Test(Test_lexer, lexer_newline_after_semicolon)
{
    char *input = "echo bonjour ttlm;\\n";
    struct lexer *lexer = lexer_new(input);

    struct token token1 = lexer_peek(lexer);
    struct token token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_SEMICOLON);
    cr_assert_eq(token2.type, TOKEN_SEMICOLON);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_NEWLINE);
    cr_assert_eq(token2.type, TOKEN_NEWLINE);

    lexer_free(lexer);
}

Test(Test_lexer, lexer_escaped_semicolon)
{
    char *input = "echo a b \\;;";
    struct lexer *lexer = lexer_new(input);

    struct token token1 = lexer_peek(lexer);
    struct token token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);
    cr_assert_str_eq(token1.value, "echo");

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);
    cr_assert_str_eq(token1.value, "a");

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);
    cr_assert_str_eq(token1.value, "b");

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);
    cr_assert_str_eq(token1.value, ";");

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_SEMICOLON);
    cr_assert_eq(token2.type, TOKEN_SEMICOLON);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_EOF);
    cr_assert_eq(token2.type, TOKEN_EOF);

    lexer_free(lexer);
}

Test(Test_lexer, lexer_simple_comment)
{
    char *input = "echo bonjour #ttlm c'est moi oui hehe";
    struct lexer *lexer = lexer_new(input);

    struct token token1 = lexer_peek(lexer);
    struct token token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);

    lexer_free(lexer);
}

Test(Test_lexer, lexer_simple_comment_newline_semicolon)
{
    char *input = "echo bonjour #ttlm c'est moi oui hehe\\n;";
    struct lexer *lexer = lexer_new(input);

    struct token token1 = lexer_peek(lexer);
    struct token token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);


    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_WORDS);
    cr_assert_eq(token2.type, TOKEN_WORDS);


    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_SEMICOLON);
    cr_assert_eq(token2.type, TOKEN_SEMICOLON);

    token1 = lexer_peek(lexer);
    token2 = lexer_pop(lexer);
    cr_assert_eq(token1.type, TOKEN_EOF);
    cr_assert_eq(token2.type, TOKEN_EOF);

    lexer_free(lexer);
}
*/
/*=========================TEST PARSER========================================*/

Test(Test_parser, simple_command_echo_test)
{
    char *input = "echo test";

    struct ast *ref = ast_new(AST_COMMAND);
    ref->ast_command = ast_command_new();
    ref->ast_command->size_command = 2;
    ref->ast_command->command_list =
        malloc(sizeof(char *) * ref->ast_command->size_command);

    ref->ast_command->command_list[0] =
        calloc(strlen("echo") + 1, sizeof(char));
    strcpy(ref->ast_command->command_list[0], "echo");

    ref->ast_command->command_list[1] =
        calloc(strlen("test") + 1, sizeof(char));
    strcpy(ref->ast_command->command_list[1], "test");

    struct lexer *lexer = lexer_new(input);
    struct ast *ast = NULL;

    cr_assert_eq(input_parse(&ast, lexer), PARSER_OK);

    cr_assert_eq(ast->ast_command->size_command,
                 ref->ast_command->size_command);

    for (size_t i = 0; i < ref->ast_command->size_command; i++)
        cr_assert_str_eq(ast->ast_command->command_list[i],
                         ref->ast_command->command_list[i]);

    ast_free(ast);
    ast_free(ref);
    lexer_free(lexer);
}
/*
Test(Test_parser, simple_command_true)
{
    char *input = "true";

    struct ast *ref = ast_new(AST_COMMAND);
    ref->size_command = 1;
    ref->command_list = malloc(sizeof(char*) * ref->size_command);

    ref->command_list[0] = calloc(strlen("true") + 1, sizeof(char));
    strcpy(ref->command_list[0], "true");

    struct lexer *lexer = lexer_new(input);
    struct ast *ast = NULL;

    cr_assert_eq(input_parse(&ast, lexer), PARSER_OK);

    cr_assert_eq(ast->size_command, ref->size_command);

    for (size_t i = 0; i < ref->size_command; i++)
        cr_assert_str_eq(ast->command_list[i], ref->command_list[i]);

    ast_free(ast);
    ast_free(ref);
    lexer_free(lexer);
}

Test(Test_parser, simple_command_echo_e_test)
{
    char *input = "echo -e test";

    struct ast *ref = ast_new(AST_COMMAND);
    ref->size_command = 3;
    ref->command_list = malloc(sizeof(char*) * ref->size_command);

    ref->command_list[0] = calloc(strlen("echo") + 1, sizeof(char));
    strcpy(ref->command_list[0], "echo");

    ref->command_list[1] = calloc(strlen("-e") + 1, sizeof(char));
    strcpy(ref->command_list[1], "-e");

    ref->command_list[2] = calloc(strlen("test") + 1, sizeof(char));
    strcpy(ref->command_list[2], "test");

    struct lexer *lexer = lexer_new(input);
    struct ast *ast = NULL;

    cr_assert_eq(input_parse(&ast, lexer), PARSER_OK);
    cr_assert_eq(ast->size_command, ref->size_command);

    for (size_t i = 0; i < ref->size_command; i++)
        cr_assert_str_eq(ast->command_list[i], ref->command_list[i]);

    ast_free(ast);
    ast_free(ref);
    lexer_free(lexer);
}

Test(Test_parser, simple_command_echo_e_test_newline)
{
    char *input = "echo -e test\\n";

    struct ast *ref = ast_new(AST_COMMAND);
    ref->size_command = 3;
    ref->command_list = malloc(sizeof(char*) * ref->size_command);

    ref->command_list[0] = calloc(strlen("echo") + 1, sizeof(char));
    strcpy(ref->command_list[0], "echo");

    ref->command_list[1] = calloc(strlen("-e") + 1, sizeof(char));
    strcpy(ref->command_list[1], "-e");

    ref->command_list[2] = calloc(strlen("test\\n") + 1, sizeof(char));
    strcpy(ref->command_list[2], "testn");

    struct lexer *lexer = lexer_new(input);
    struct ast *ast = NULL;

    cr_assert_eq(input_parse(&ast, lexer), PARSER_OK);
    cr_assert_eq(ast->size_command, ref->size_command);

    for (size_t i = 0; i < ref->size_command; i++)
        cr_assert_str_eq(ast->command_list[i], ref->command_list[i]);

    ast_free(ast);
    ast_free(ref);
    lexer_free(lexer);
}

Test(Test_parser, if_else)
{
    char *input = "if true; then echo a; else echo b; fi";

    struct ast *ref = ast_new(AST_IF);
    ref->body_if = ast_new(AST_COMMAND);
    ref->cond = ast_new(AST_COMMAND);
    ref->body_else = ast_new(AST_COMMAND);

    ref->cond->size_command = 1;
    ref->cond->command_list = malloc(sizeof(char*) * ref->cond->size_command);
    ref->cond->command_list[0] = calloc(strlen("true") + 1, sizeof(char));
    strcpy(ref->cond->command_list[0], "true");

    ref->body_if->size_command = 2;
    ref->body_if->command_list = malloc(sizeof(char*) *
ref->body_if->size_command); ref->body_if->command_list[0] =
calloc(strlen("echo") + 1, sizeof(char)); strcpy(ref->body_if->command_list[0],
"echo"); ref->body_if->command_list[1] = calloc(strlen("a") + 1, sizeof(char));
    strcpy(ref->body_if->command_list[1], "a");

    ref->body_else->size_command = 2;
    ref->body_else->command_list = malloc(sizeof(char*) *
ref->body_else->size_command); ref->body_else->command_list[0] =
calloc(strlen("echo") + 1, sizeof(char));
    strcpy(ref->body_else->command_list[0], "echo");
    ref->body_else->command_list[1] = calloc(strlen("b") + 1, sizeof(char));
    strcpy(ref->body_else->command_list[1], "b");

    struct lexer *lexer = lexer_new(input);
    struct ast *ast = NULL;

    cr_assert_eq(input_parse(&ast, lexer), PARSER_OK);

    cr_assert_eq(ast->body_if->size_command, ref->body_if->size_command);
    cr_assert_eq(ast->cond->size_command, ref->cond->size_command);
    cr_assert_eq(ast->body_else->size_command, ref->body_else->size_command);

    for (size_t i = 0; i < ref->size_command; i++)
    {
        cr_assert_str_eq(ast->body_if->command_list[i],
ref->body_if->command_list[i]);
        cr_assert_str_eq(ast->body_else->command_list[i],
ref->body_else->command_list[i]); cr_assert_str_eq(ast->cond->command_list[i],
ref->cond->command_list[i]);
    }

    ast_free(ast);
    ast_free(ref);
    lexer_free(lexer);
}

Test(Test_parser, if_else_2_with_newline)
{
    char *input = "if true;\\n then echo a;\\n else echo b;\\n fi\\n";

    struct ast *ref = ast_new(AST_IF);
    ref->body_if = ast_new(AST_COMMAND);
    ref->cond = ast_new(AST_COMMAND);
    ref->body_else = ast_new(AST_COMMAND);

    ref->cond->size_command = 1;
    ref->cond->command_list = malloc(sizeof(char*) * ref->cond->size_command);
    ref->cond->command_list[0] = calloc(strlen("true") + 1, sizeof(char));
    strcpy(ref->cond->command_list[0], "true");

    ref->body_if->size_command = 2;
    ref->body_if->command_list = malloc(sizeof(char*) *
ref->body_if->size_command); ref->body_if->command_list[0] =
calloc(strlen("echo") + 1, sizeof(char)); strcpy(ref->body_if->command_list[0],
"echo"); ref->body_if->command_list[1] = calloc(strlen("a") + 1, sizeof(char));
    strcpy(ref->body_if->command_list[1], "a");

    ref->body_else->size_command = 2;
    ref->body_else->command_list = malloc(sizeof(char*) *
ref->body_else->size_command); ref->body_else->command_list[0] =
calloc(strlen("echo") + 1, sizeof(char));
    strcpy(ref->body_else->command_list[0], "echo");
    ref->body_else->command_list[1] = calloc(strlen("b") + 1, sizeof(char));
    strcpy(ref->body_else->command_list[1], "b");

    struct lexer *lexer = lexer_new(input);
    struct ast *ast = NULL;

    cr_assert_eq(input_parse(&ast, lexer), PARSER_OK);

    cr_assert_eq(ast->body_if->size_command, ref->body_if->size_command);
    cr_assert_eq(ast->cond->size_command, ref->cond->size_command);
    cr_assert_eq(ast->body_else->size_command, ref->body_else->size_command);

    for (size_t i = 0; i < ref->size_command; i++)
    {
        cr_assert_str_eq(ast->body_if->command_list[i],
ref->body_if->command_list[i]);
        cr_assert_str_eq(ast->body_else->command_list[i],
ref->body_else->command_list[i]); cr_assert_str_eq(ast->cond->command_list[i],
ref->cond->command_list[i]);
    }

    ast_free(ast);
    ast_free(ref);
    lexer_free(lexer);
}

Test(Test_parser, 2_simple_command_)
{
    char *input = "echo test; echo oui;";

    struct ast *ref = ast_new(AST_COMMAND);
    ref->size_command = 2;
    ref->command_list = malloc(sizeof(char*) * ref->size_command);

    ref->command_list[0] = calloc(strlen("echo") + 1, sizeof(char));
    strcpy(ref->command_list[0], "echo");

    ref->command_list[1] = calloc(strlen("test") + 1, sizeof(char));
    strcpy(ref->command_list[1], "test");

    struct ast *ref2 = ast_new(AST_COMMAND);
    ref2->ast_command->size_command = 2;
    ref2->ast_command->command_list = malloc(sizeof(char*) *
ref2->size_command);

    ref2->ast_command->command_list[0] = calloc(strlen("echo") + 1,
sizeof(char)); strcpy(ref2->ast_command->command_list[0], "echo");

    ref2->ast_command->command_list[1] = calloc(strlen("oui") + 1,
sizeof(char)); strcpy(ref2->ast_command->command_list[1], "oui");

    ref->next_command = ref2;

    struct lexer *lexer = lexer_new(input);
    struct ast *ast = NULL;

    cr_assert_eq(input_parse(&ast, lexer), PARSER_OK);

    cr_assert_eq(ast->size_command, ref->size_command);

    for (size_t i = 0; i < ref->size_command; i++)
        cr_assert_str_eq(ast->command_list[i], ref->command_list[i]);

    for (size_t i = 0; i < ref2->ast_command->size_command; i++)
        cr_assert_str_eq(ast->next_command->command_list[i],
ref2->ast_command->command_list[i]);

    ast_free(ast);
    ast_free(ref);
    lexer_free(lexer);
}

Test(Test_parser, 3_simple_command_)
{
    char *input = "echo test; echo oui; echo non;";

    struct ast *ref = ast_new(AST_COMMAND);
    ref->size_command = 2;
    ref->command_list = malloc(sizeof(char*) * ref->size_command);

    ref->command_list[0] = calloc(strlen("echo") + 1, sizeof(char));
    strcpy(ref->command_list[0], "echo");

    ref->command_list[1] = calloc(strlen("test") + 1, sizeof(char));
    strcpy(ref->command_list[1], "test");

    struct ast *ref2 = ast_new(AST_COMMAND);
    ref2->ast_command->size_command = 2;
    ref2->ast_command->command_list = malloc(sizeof(char*) *
ref2->size_command);

    ref2->ast_command->command_list[0] = calloc(strlen("echo") + 1,
sizeof(char)); strcpy(ref2->ast_command->command_list[0], "echo");

    ref2->ast_command->command_list[1] = calloc(strlen("oui") + 1,
sizeof(char)); strcpy(ref2->ast_command->command_list[1], "oui");

    struct ast *ref3 = ast_new(AST_COMMAND);
    ref3->ast_command->size_command = 2;
    ref3->ast_command->command_list = malloc(sizeof(char*) *
ref3->size_command);

    ref3->ast_command->command_list[0] = calloc(strlen("echo") + 1,
sizeof(char)); strcpy(ref3->ast_command->command_list[0], "echo");

    ref3->ast_command->command_list[1] = calloc(strlen("non") + 1,
sizeof(char)); strcpy(ref3->ast_command->command_list[1], "non");

    struct lexer *lexer = lexer_new(input);
    struct ast *ast = NULL;

    cr_assert_eq(input_parse(&ast, lexer), PARSER_OK);

    cr_assert_eq(ast->size_command, ref->size_command);

    for (size_t i = 0; i < ref->size_command; i++)
        cr_assert_str_eq(ast->command_list[i], ref->command_list[i]);

    for (size_t i = 0; i < ref2->ast_command->size_command; i++)
        cr_assert_str_eq(ast->next_command->command_list[i],
ref2->ast_command->command_list[i]);

    for (size_t i = 0; i < ref3->ast_command->size_command; i++)
        cr_assert_str_eq(ast->next_command->next_command->command_list[i],
ref3->ast_command->command_list[i]);

    ast_free(ast);
    ast_free(ref);
    lexer_free(lexer);
}
*/
/*=======================TEST EVAL_AST========================================*/
