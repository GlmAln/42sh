#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

struct ast *ast_new(enum ast_type type)
{
    struct ast *new = calloc(1, sizeof(struct ast));
    if (!new)
        return NULL;

    new->type = type;
    new->negation = 0;
    new->next = NULL;
    new->next_and_or = NULL;
    new->next_pipe = NULL;

    new->ast_command = NULL;
    new->ast_if = NULL;
    new->ast_for = NULL;
    new->ast_while = NULL;
    new->ast_until = NULL;
    new->ast_redirect = NULL;

    return new;
}

struct ast_for *ast_for_new(void)
{
    struct ast_for *new = calloc(1, sizeof(struct ast_for));
    if (!new)
        return NULL;

    new->body_for = NULL;
    new->body_in = NULL;
    new->str_in = NULL;
    new->size_in = 0;
    new->body_element = NULL;
    new->str_element = NULL;
    new->size_redir = 0;
    new->redir_list = NULL;
    return new;
}

struct ast_while *ast_while_new(void)
{
    struct ast_while *new = calloc(1, sizeof(struct ast_while));
    if (!new)
        return NULL;

    new->body_cond = NULL;
    new->body_while = NULL;
    new->size_redir = 0;
    new->redir_list = NULL;
    return new;
}

struct ast_until *ast_until_new(void)
{
    struct ast_until *new = calloc(1, sizeof(struct ast_until));
    if (!new)
        return NULL;

    new->body_cond = NULL;
    new->body_until = NULL;
    new->size_redir = 0;
    new->redir_list = NULL;
    return new;
}

struct ast_if *ast_if_new(void)
{
    struct ast_if *new = calloc(1, sizeof(struct ast_if));
    if (!new)
        return NULL;

    new->body_cond = NULL;
    new->body_then = NULL;
    new->body_else = NULL;
    new->size_redir = 0;
    new->redir_list = NULL;
    return new;
}

struct ast_command *ast_command_new(void)
{
    struct ast_command *new = calloc(1, sizeof(struct ast_command));
    if (!new)
        return NULL;

    new->token_list = NULL;
    new->str_list = NULL;
    new->size_token = 0;
    new->assign_list = NULL;
    new->size_assign = 0;
    new->redir_list = NULL;
    new->size_redir = 0;
    return new;
}

struct redir *redir_new(void)
{
    struct redir *new = calloc(1, sizeof(struct redir));
    if (!new)
        return NULL;

    new->or_symbole = 0;
    new->and_symbole = 0;
    new->file = NULL;
    new->str_file = NULL;
    new->io_number = NULL;
    return new;
}

void redir_free(struct redir *redir)
{
    if (!redir)
        return;

    free(redir->str_file);
    free(redir);
    redir = NULL;
}

static void ast_for_free(struct ast_for *ast_for, struct ast **res)
{
    if (!ast_for)
        return;

    ast_free(ast_for->body_for, res);
    for (size_t i = 0; i < ast_for->size_in; i++)
        free(ast_for->str_in[i]);
    free(ast_for->str_in);
    free(ast_for->str_element);
    free(ast_for->body_in);
    for (size_t i = 0; i < ast_for->size_redir; i++)
        redir_free(ast_for->redir_list[i]);
    free(ast_for->redir_list);
    free(ast_for);
}

static void ast_command_free(struct ast_command *ast_command)
{
    if (!ast_command)
        return;

    free(ast_command->token_list);
    free(ast_command->assign_list);

    for (size_t i = 0; i < ast_command->size_token; i++)
        free(ast_command->str_list[i]);
    for (size_t i = 0; i < ast_command->size_assign; i++)
        free(ast_command->str_assign[i]);
    for (size_t i = 0; i < ast_command->size_redir; i++)
        redir_free(ast_command->redir_list[i]);

    free(ast_command->str_list);
    free(ast_command->str_assign);
    free(ast_command->redir_list);

    free(ast_command);
    ast_command = NULL;
}

static void ast_while_free(struct ast_while *ast_while, struct ast **res)
{
    if (!ast_while)
        return;

    ast_free(ast_while->body_while, res);
    ast_free(ast_while->body_cond, res);
    for (size_t i = 0; i < ast_while->size_redir; i++)
        redir_free(ast_while->redir_list[i]);
    free(ast_while->redir_list);
    free(ast_while);
}

static void ast_until_free(struct ast_until *ast_until, struct ast **res)
{
    if (!ast_until)
        return;

    ast_free(ast_until->body_until, res);
    ast_free(ast_until->body_cond, res);
    for (size_t i = 0; i < ast_until->size_redir; i++)
        redir_free(ast_until->redir_list[i]);
    free(ast_until->redir_list);
    free(ast_until);
}

static void ast_if_free(struct ast_if *ast_if, struct ast **res)
{
    if (!ast_if)
        return;

    ast_free(ast_if->body_then, res);
    ast_free(ast_if->body_cond, res);
    ast_free(ast_if->body_else, res);
    for (size_t i = 0; i < ast_if->size_redir; i++)
        redir_free(ast_if->redir_list[i]);
    free(ast_if->redir_list);
    ast_if->body_then = NULL;
    ast_if->body_else = NULL;
    ast_if->body_cond = NULL;
    free(ast_if);
    ast_if = NULL;
}

void ast_free(struct ast *ast, struct ast **res)
{
    if (!ast)
        return;

    ast_until_free(ast->ast_until, res);
    ast->ast_until = NULL;

    ast_while_free(ast->ast_while, res);
    ast->ast_while = NULL;

    ast_if_free(ast->ast_if, res);
    ast->ast_if = NULL;

    ast_command_free(ast->ast_command);
    ast->ast_command = NULL;

    ast_for_free(ast->ast_for, res);
    ast->ast_for = NULL;

    ast_free(ast->next, res);
    ast_free(ast->next_and_or, res);
    ast_free(ast->next_pipe, res);

    if (res == NULL || *res != ast)
    {
        free(ast);
        ast = NULL;
    }
}
