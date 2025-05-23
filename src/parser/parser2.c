#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

static void update_redirect(struct ast *t, struct redir *new)
{
    if (t->type == AST_IF)
    {
        size_t size = ++t->ast_if->size_redir;
        t->ast_if->redir_list =
            realloc(t->ast_if->redir_list, size * sizeof(struct redir *));
        t->ast_if->redir_list[size - 1] = new;
    }
    else if (t->type == AST_COMMAND)
    {
        size_t size = ++t->ast_command->size_redir;
        t->ast_command->redir_list =
            realloc(t->ast_command->redir_list, size * sizeof(struct redir *));
        t->ast_command->redir_list[size - 1] = new;
    }
    else if (t->type == AST_WHILE)
    {
        size_t size = ++t->ast_while->size_redir;
        t->ast_while->redir_list =
            realloc(t->ast_while->redir_list, size * sizeof(struct redir *));
        t->ast_while->redir_list[size - 1] = new;
    }
    else if (t->type == AST_UNTIL)
    {
        size_t size = ++t->ast_until->size_redir;
        t->ast_until->redir_list =
            realloc(t->ast_until->redir_list, size * sizeof(struct redir *));
        t->ast_until->redir_list[size - 1] = new;
    }
    else if (t->type == AST_FOR)
    {
        size_t size = ++t->ast_for->size_redir;
        t->ast_for->redir_list =
            realloc(t->ast_for->redir_list, size * sizeof(struct redir *));
        t->ast_for->redir_list[size - 1] = new;
    }
}

static void free_all(struct ast **ref, struct ast *ast1, struct ast *ast2,
                     struct ast *ast3)
{
    ast_free(ast1, ref);
    ast_free(ast2, ref);
    ast_free(ast3, ref);
    ast1 = NULL;
    ast2 = NULL;
    ast3 = NULL;
}

enum parser_status shell_command_parse(struct ast **res, struct lexer *lexer,
                                       struct function *func)
{
    enum parser_status result = PARSER_UNEXPECTED_TOKEN;

    if (!strcmp((lexer_peek(lexer))->str, "{"))
    {
        lexer_pop(lexer);
        if (compound_list_parse(res, lexer, func) == PARSER_OK)
        {
            if (!strcmp((lexer_peek(lexer))->str, "}"))
            {
                lexer_pop(lexer);
                result = PARSER_OK;
            }
        }
    }

    else if (!strcmp((lexer_peek(lexer))->str, "(")
             && strcmp((lexer_peek2(lexer))->str, ")"))
    {
        lexer_pop(lexer);
        if (compound_list_parse(res, lexer, func) == PARSER_OK)
        {
            if (!strcmp((lexer_peek(lexer))->str, ")"))
            {
                lexer_pop(lexer);
                result = PARSER_OK;
            }
        }
    }

    else if (rule_if_parse(res, lexer, func) == PARSER_OK)
        result = PARSER_OK;
    else if (rule_while_parse(res, lexer, func) == PARSER_OK)
        result = PARSER_OK;
    else if (rule_until_parse(res, lexer, func) == PARSER_OK)
        result = PARSER_OK;
    else if (rule_for_parse(res, lexer, func) == PARSER_OK)
        result = PARSER_OK;

    if (result == PARSER_UNEXPECTED_TOKEN)
        return PARSER_UNEXPECTED_TOKEN;

    while (redirection_parse(res, lexer, func) == PARSER_OK)
        continue;
    return PARSER_OK;
}

enum parser_status redirection_parse(struct ast **res, struct lexer *lexer,
                                     struct function *func)
{
    struct redir *new = redir_new();
    if ((lexer_peek(lexer))->type == IO_NUMBER)
        new->io_number = (lexer_pop(lexer))->str;

    struct token *tok = lexer_peek(lexer);
    if (!strcmp(tok->str, ">") || !strcmp(tok->str, "<")
        || !strcmp(tok->str, "<<") || !strcmp(tok->str, ">>")
        || !strcmp(tok->str, "<>") || !strcmp(tok->str, ">&")
        || !strcmp(tok->str, ">|") || !strcmp(tok->str, "<&"))
    {
        lexer_pop(lexer);
        new->redirect = tok->str;
        if (!strcmp(tok->str, ">&") || !strcmp(tok->str, "<&"))
            new->and_symbole = 1;

        if (!strcmp(tok->str, ">|"))
            new->or_symbole = 1;

        if (is_word(lexer_peek(lexer)))
        {
            struct token *tok = lexer_pop(lexer);
            new->file = tok;
            new->str_file = calloc(strlen(tok->str) + 1, sizeof(char));
            strcpy(new->str_file, tok->str);
        }

        else
        {
            redir_free(new);
            return PARSER_UNEXPECTED_TOKEN;
        }

        if (*res == NULL)
        {
            *res = ast_new(AST_COMMAND);
            (*res)->ast_command = ast_command_new();
        }

        update_redirect(*res, new);
        return PARSER_OK;
    }
    else
    {
        redir_free(new);
        return PARSER_UNEXPECTED_TOKEN;
    }

    return (func) ? PARSER_OK : PARSER_OK; // func unused
}

enum parser_status rule_while_parse(struct ast **res, struct lexer *lexer,
                                    struct function *func)
{
    if (!strcmp((lexer_peek(lexer))->str, "while"))
    {
        struct ast *new = ast_new(AST_WHILE);
        new->ast_while = ast_while_new();
        struct ast *tmp = *res;

        lexer_pop(lexer);
        if (compound_list_parse(res, lexer, func) != PARSER_OK)
            goto free_all;

        new->ast_while->body_cond = *res;
        if (!strcmp((lexer_peek(lexer))->str, "do"))
        {
            tmp = *res;
            lexer_pop(lexer);
            if (compound_list_parse(res, lexer, func) == PARSER_OK)
                new->ast_while->body_while = *res;
            else
                goto free_all;
        }
        else
            goto free_all;

        tmp = *res;
        if (!strcmp((lexer_peek(lexer))->str, "done"))
        {
            lexer_pop(lexer);
            *res = new;
            return PARSER_OK;
        }
    free_all:
        free_all(res, tmp, new, NULL);
    }
    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status rule_until_parse(struct ast **res, struct lexer *lexer,
                                    struct function *func)
{
    if (!strcmp((lexer_peek(lexer))->str, "until"))
    {
        struct ast *new = ast_new(AST_UNTIL);
        new->ast_until = ast_until_new();
        struct ast *tmp = *res;

        lexer_pop(lexer);
        if (compound_list_parse(res, lexer, func) != PARSER_OK)
            goto free_all;

        new->ast_until->body_cond = *res;
        if (!strcmp((lexer_peek(lexer))->str, "do"))
        {
            tmp = *res;
            lexer_pop(lexer);
            if (compound_list_parse(res, lexer, func) == PARSER_OK)
                new->ast_until->body_until = *res;
            else
                goto free_all;
        }
        else
            goto free_all;

        if (!strcmp((lexer_peek(lexer))->str, "done"))
        {
            lexer_pop(lexer);
            *res = new;
            return PARSER_OK;
        }
    free_all:
        free_all(res, tmp, new, NULL);
    }
    return PARSER_UNEXPECTED_TOKEN;
}

static void update_body_in(struct ast *new, struct lexer *lexer,
                           struct token *tok)
{
    while (is_word(lexer_peek(lexer)))
    {
        size_t n = ++new->ast_for->size_in;
        new->ast_for->body_in =
            realloc(new->ast_for->body_in, sizeof(struct token *) * n);
        tok = lexer_pop(lexer);
        new->ast_for->body_in[n - 1] = tok;
        new->ast_for->str_in =
            realloc(new->ast_for->str_in, sizeof(char *) * n);
        new->ast_for->str_in[n - 1] =
            calloc(strlen(tok->str) + 1, sizeof(char));
        strcpy(new->ast_for->str_in[n - 1], tok->str);
    }
}

static struct token *update_body_element(struct ast *new, struct lexer *lexer)
{
    struct token *tok = lexer_pop(lexer);
    new->ast_for->body_element = tok;
    new->ast_for->str_element = calloc(strlen(tok->str) + 1, sizeof(char));
    strcpy(new->ast_for->str_element, tok->str);

    return tok;
}

enum parser_status rule_for_parse(struct ast **res, struct lexer *lexer,
                                  struct function *func)
{
    if (!strcmp((lexer_peek(lexer))->str, "for"))
    {
        struct ast *new = ast_new(AST_FOR);

        new->ast_for = ast_for_new();
        struct ast *tmp = *res;

        lexer_pop(lexer);
        if (is_word(lexer_peek(lexer)))
        {
            struct token *tok = update_body_element(new, lexer);
            if (!strcmp((lexer_peek(lexer))->str, ";"))
            {
                new->ast_for->body_in = NULL;
                lexer_pop(lexer);
            }
            else if (!strcmp((lexer_peek(lexer))->str, "\n")
                     || !strcmp((lexer_peek(lexer))->str, "in"))
            {
                while (!strcmp((lexer_peek(lexer))->str, "\n"))
                    lexer_pop(lexer);

                if (!strcmp((lexer_peek(lexer))->str, "in"))
                {
                    lexer_pop(lexer);
                    update_body_in(new, lexer, tok);
                    if (strcmp((lexer_peek(lexer))->str, "\n")
                        && strcmp((lexer_peek(lexer))->str, ";"))
                        goto free_all;
                    lexer_pop(lexer);
                }
                else
                    goto free_all;
            }

            while (!strcmp((lexer_peek(lexer))->str, "\n"))
                lexer_pop(lexer);

            if (strcmp((lexer_peek(lexer))->str, "do"))
                goto free_all;
            lexer_pop(lexer);

            if (compound_list_parse(res, lexer, func) == PARSER_OK)
            {
                new->ast_for->body_for = *res;
                if (!strcmp((lexer_peek(lexer))->str, "done"))
                {
                    lexer_pop(lexer);
                    *res = new;
                    return PARSER_OK;
                }
            }
        free_all:
            free_all(res, tmp, new, NULL);
        }
    }
    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status rule_if_parse(struct ast **res, struct lexer *lexer,
                                 struct function *func)
{
    if (!strcmp((lexer_peek(lexer))->str, "if"))
    {
        struct ast *new = ast_new(AST_IF);
        new->ast_if = ast_if_new();
        struct ast *tmp = *res;

        lexer_pop(lexer);
        if (compound_list_parse(res, lexer, func) == PARSER_OK)
            new->ast_if->body_cond = *res;
        else
            goto free_all;

        tmp = *res;
        if (!strcmp((lexer_peek(lexer))->str, "then"))
        {
            lexer_pop(lexer);
            if (compound_list_parse(res, lexer, func) == PARSER_OK)
                new->ast_if->body_then = *res;
            else
                goto free_all;
        }
        else
            goto free_all;

        tmp = *res;
        if (else_clause_parse(res, lexer, func) == PARSER_OK)
            new->ast_if->body_else = *res;

        tmp = *res;
        if (!strcmp((lexer_peek(lexer))->str, "fi"))
        {
            lexer_pop(lexer);
            *res = new;
            return PARSER_OK;
        }

    free_all:
        free_all(res, tmp, new, NULL);
    }
    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status else_clause_parse(struct ast **res, struct lexer *lexer,
                                     struct function *func)
{
    if (!strcmp((lexer_peek(lexer))->str, "elif"))
    {
        struct ast *new = ast_new(AST_ELIF);
        new->ast_if = ast_if_new();
        struct ast *tmp = *res;

        lexer_pop(lexer);
        if (compound_list_parse(res, lexer, func) == PARSER_OK)
            new->ast_if->body_cond = *res;
        else
            goto free_all;

        tmp = *res;
        if (!strcmp((lexer_peek(lexer))->str, "then"))
        {
            lexer_pop(lexer);
            if (compound_list_parse(res, lexer, func) == PARSER_OK)
                new->ast_if->body_then = *res;
            else
                goto free_all;
        }
        else
            goto free_all;

        if (else_clause_parse(res, lexer, func) == PARSER_OK)
            new->ast_if->body_else = *res;

        *res = new;
        return PARSER_OK;

    free_all:
        free_all(res, tmp, new, NULL);
    }

    else if (!strcmp((lexer_peek(lexer))->str, "else"))
    {
        lexer_pop(lexer);
        return compound_list_parse(res, lexer, func);
    }

    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status compound_list_parse(struct ast **res, struct lexer *lexer,
                                       struct function *func)
{
    while (!strcmp((lexer_peek(lexer))->str, "\n"))
        lexer_pop(lexer);

    if (and_or_parse(res, lexer, func) == PARSER_OK)
    {
        struct ast *new = *res;
        struct ast **tmp = &new->next;
        while (!strcmp((lexer_peek(lexer))->str, ";")
               || !strcmp((lexer_peek(lexer))->str, "\n"))
        {
            lexer_pop(lexer);
            while (!strcmp((lexer_peek(lexer))->str, "\n"))
                lexer_pop(lexer);

            if (and_or_parse(tmp, lexer, func) == PARSER_UNEXPECTED_TOKEN)
                break;

            if ((*tmp) != NULL)
                tmp = &(*tmp)->next;
        }
        *res = new;

        if (!strcmp((lexer_peek(lexer))->str, ";"))
            lexer_pop(lexer);

        while (!strcmp((lexer_peek(lexer))->str, "\n"))
            lexer_pop(lexer);
    }
    else
        return PARSER_UNEXPECTED_TOKEN;
    return PARSER_OK;
}

enum parser_status funcdec_parse(struct ast **res, struct lexer *lexer,
                                 struct function *func)
{
    if (not_reserved(lexer_peek(lexer))
        && !strcmp((lexer_peek2(lexer))->str, "("))
    {
        struct token *key = lexer_pop(lexer);
        if (!strcmp((lexer_peek(lexer))->str, "(")
            && !strcmp((lexer_peek2(lexer))->str, ")"))
        {
            lexer_pop(lexer);
            lexer_pop(lexer);
            while (!strcmp((lexer_peek(lexer))->str, "\n"))
                lexer_pop(lexer);

            struct ast *new = NULL;
            if (shell_command_parse(&new, lexer, func) == PARSER_OK)
            {
                function_set(func, key->str, new);
                return PARSER_OK;
            }
            else
                ast_free(new, res);
        }
    }
    return PARSER_UNEXPECTED_TOKEN;
}
