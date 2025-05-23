#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

int not_reserved(struct token *tok)
{
    return (strcmp(tok->str, "if") && strcmp(tok->str, "for")
            && strcmp(tok->str, ",") && strcmp(tok->str, "\n")
            && strcmp(tok->str, "&") && strcmp(tok->str, "&&")
            && strcmp(tok->str, "|") && strcmp(tok->str, "||")
            && strcmp(tok->str, "else") && strcmp(tok->str, "elif")
            && strcmp(tok->str, "do") && strcmp(tok->str, "done")
            && strcmp(tok->str, "in") && strcmp(tok->str, "while")
            && strcmp(tok->str, "until") && strcmp(tok->str, ">>")
            && strcmp(tok->str, "<") && strcmp(tok->str, ">")
            && strcmp(tok->str, "<&") && strcmp(tok->str, "&>")
            && strcmp(tok->str, ">|") && strcmp(tok->str, "<>")
            && strcmp(tok->str, "<<") && strcmp(tok->str, "<<-")
            && strcmp(tok->str, "!") && strcmp(tok->str, "case")
            && strcmp(tok->str, "esac") && strcmp(tok->str, "(")
            && strcmp(tok->str, ")") && strcmp(tok->str, "{")
            && strcmp(tok->str, "}") && strcmp(tok->str, ";;")
            && strcmp(tok->str, "then") && strcmp(tok->str, "fi")
            && strcmp(tok->str, "") && strcmp(tok->str, "\26")
            && strcmp(tok->str, ";"));
}

int is_word(struct token *tok)
{
    return (strcmp(tok->str, ",") && strcmp(tok->str, "\n")
            && strcmp(tok->str, "&") && strcmp(tok->str, "&&")
            && strcmp(tok->str, "|") && strcmp(tok->str, "||")
            && strcmp(tok->str, ">>") && strcmp(tok->str, ";")
            && strcmp(tok->str, "<") && strcmp(tok->str, ">")
            && strcmp(tok->str, "<&") && strcmp(tok->str, "&>")
            && strcmp(tok->str, ">|") && strcmp(tok->str, "<>")
            && strcmp(tok->str, "<<") && strcmp(tok->str, "<<-")
            && strcmp(tok->str, "!") && strcmp(tok->str, "(")
            && strcmp(tok->str, ")") && strcmp(tok->str, "{")
            && strcmp(tok->str, "}") && strcmp(tok->str, ";;")
            && strcmp(tok->str, "") && strcmp(tok->str, "\26")
            && tok->type == WORD);
}

enum parser_status input_parse(struct ast **res, struct lexer *lexer,
                               struct function *func, int *eof)
{
    struct token *peek = lexer_peek(lexer);
    if (peek->str[0] == EOF || peek->str[0] == 0)
        lexer_pop(lexer);

    else if (!strcmp(peek->str, "\n"))
        lexer_pop(lexer);

    else if (list_parse(res, lexer, func) == PARSER_OK
             && (!strcmp((lexer_peek(lexer))->str, "\n")
                 || ((lexer_peek(lexer))->str[0] == EOF
                     || ((lexer_peek(lexer))->str[0] == 0))))
        lexer_pop(lexer);

    else
    {
        fprintf(stderr, "42sh: syntax error unexpected token\n");
        ast_free(*res, NULL);
        return PARSER_UNEXPECTED_TOKEN;
    }
    peek = lexer_peek(lexer);
    *eof = peek->type == EOF;
    return PARSER_OK;
}

enum parser_status list_parse(struct ast **res, struct lexer *lexer,
                              struct function *func)
{
    if (and_or_parse(res, lexer, func) == PARSER_OK)
    {
        if (*res == NULL && !function_is_empty(func))
            *res = ast_new(AST_COMMAND);

        if (!strcmp((lexer_peek(lexer))->str, ";"))
        {
            struct ast *new = *res;
            struct ast **tmp = &new->next;

            while (!strcmp((lexer_peek(lexer))->str, ";"))
            {
                lexer_pop(lexer);
                if (and_or_parse(tmp, lexer, func) != PARSER_OK)
                    break;
                if ((*tmp) != NULL)
                    tmp = &(*tmp)->next;
            }
            *res = new;
        }
    }
    else
        return PARSER_UNEXPECTED_TOKEN;
    return PARSER_OK;
}

enum parser_status and_or_parse(struct ast **res, struct lexer *lexer,
                                struct function *func)
{
    if (pipeline_parse(res, lexer, func) == PARSER_OK)
    {
        if (*res == NULL && !function_is_empty(func))
        {
            *res = ast_new(AST_COMMAND);
        }

        struct ast *new = *res;
        struct ast **tmp = &new->next_and_or;
        new->and_or_symbole = !strcmp((lexer_peek(lexer))->str, "&&") ? 1 : 2;

        while (!strcmp((lexer_peek(lexer))->str, "&&")
               || !strcmp((lexer_peek(lexer))->str, "||"))
        {
            char *tok = (lexer_pop(lexer)->str);

            while (!strcmp((lexer_peek(lexer))->str, "\n"))
                lexer_pop(lexer);

            if (pipeline_parse(tmp, lexer, func) != PARSER_OK)
                return PARSER_UNEXPECTED_TOKEN;

            (*tmp)->and_or_symbole = (!strcmp(tok, "&&")) ? 1 : 2;
            if ((*tmp) != NULL)
                tmp = &(*tmp)->next_and_or;
        }
        *res = new;
    }
    else
        return PARSER_UNEXPECTED_TOKEN;
    return PARSER_OK;
}

enum parser_status pipeline_parse(struct ast **res, struct lexer *lexer,
                                  struct function *func)
{
    int negation = 0;
    if (!strcmp((lexer_peek(lexer))->str, "!"))
    {
        negation = 1;
        lexer_pop(lexer);
    }

    if (command_parse(res, lexer, func) == PARSER_OK)
    {
        if (*res == NULL && !function_is_empty(func))
            *res = ast_new(AST_COMMAND);

        struct ast *new = *res;
        new->negation = negation;
        struct ast **tmp = &new->next_pipe;
        while (!strcmp((lexer_peek(lexer))->str, "|"))
        {
            new->type = AST_PIPE;
            lexer_pop(lexer);
            while (!strcmp((lexer_peek(lexer))->str, "\n"))
                lexer_pop(lexer);

            if (command_parse(tmp, lexer, func) != PARSER_OK)
                return PARSER_UNEXPECTED_TOKEN;

            if (tmp != NULL)
                tmp = &(*tmp)->next_pipe;
        }
        *res = new;
        return PARSER_OK;
    }
    else
        return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status command_parse(struct ast **res, struct lexer *lexer,
                                 struct function *func)
{
    if (funcdec_parse(res, lexer, func) == PARSER_OK)
    {
        while ((redirection_parse(res, lexer, func)) == PARSER_OK)
            continue;
        return PARSER_OK;
    }

    else if (simple_command_parse(res, lexer, func) == PARSER_OK)
        return PARSER_OK;

    else if (shell_command_parse(res, lexer, func) == PARSER_OK)
    {
        while (redirection_parse(res, lexer, func) == PARSER_OK)
            continue;
        return PARSER_OK;
    }

    else
        return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status simple_command_parse(struct ast **res, struct lexer *lexer,
                                        struct function *func)
{
    int prefix_ok = 0;
    while (prefix_parse(res, lexer, func) == PARSER_OK)
        prefix_ok = 1;

    if (not_reserved(lexer_peek(lexer)))
    {
        *res = ast_new(AST_COMMAND);
        (*res)->ast_command = ast_command_new();
        (*res)->ast_command->size_token = 1;
        (*res)->ast_command->token_list =
            realloc((*res)->ast_command->token_list, sizeof(struct token *));

        (*res)->ast_command->str_list =
            realloc((*res)->ast_command->str_list, sizeof(char *));
        struct token *tok = lexer_pop(lexer);
        (*res)->ast_command->token_list[0] = tok;
        (*res)->ast_command->str_list[0] =
            calloc(strlen(tok->str) + 1, sizeof(char));
        strcpy((*res)->ast_command->str_list[0], tok->str);

        while (element_parse(res, lexer, func) == PARSER_OK)
            continue;
    }

    else if (prefix_ok == 0)
        return PARSER_UNEXPECTED_TOKEN;

    return PARSER_OK;
}

enum parser_status prefix_parse(struct ast **res, struct lexer *lexer,
                                struct function *func)
{
    if ((lexer_peek(lexer))->type == ASSIGNMENT_WORD)
    {
        if (*res == NULL) // else it's already an ast_command
        {
            *res = ast_new(AST_COMMAND);
            (*res)->ast_command = ast_command_new();
        }

        size_t size = ++(*res)->ast_command->size_assign;
        (*res)->ast_command->assign_list = realloc(
            (*res)->ast_command->assign_list, size * sizeof(struct token *));

        (*res)->ast_command->str_assign =
            realloc((*res)->ast_command->str_assign, size * sizeof(char *));

        struct token *tok = lexer_pop(lexer);
        (*res)->ast_command->assign_list[size - 1] = tok;
        (*res)->ast_command->str_assign[size - 1] =
            calloc(strlen(tok->str) + 1, sizeof(char));
        strcpy((*res)->ast_command->str_assign[size - 1], tok->str);
        return PARSER_OK;
    }

    else
        return redirection_parse(res, lexer, func);
}

enum parser_status element_parse(struct ast **res, struct lexer *lexer,
                                 struct function *func)
{
    if (is_word(lexer_peek(lexer)))
    {
        size_t size = ++(*res)->ast_command->size_token;
        (*res)->ast_command->token_list = realloc(
            (*res)->ast_command->token_list, size * sizeof(struct token *));

        (*res)->ast_command->str_list =
            realloc((*res)->ast_command->str_list, size * sizeof(char *));

        struct token *tok = lexer_pop(lexer);
        (*res)->ast_command->token_list[size - 1] = tok;
        (*res)->ast_command->str_list[size - 1] =
            calloc(strlen(tok->str) + 1, sizeof(char));
        strcpy((*res)->ast_command->str_list[size - 1], tok->str);
        return PARSER_OK;
    }
    else if (prefix_parse(res, lexer, func) == PARSER_OK) //
        return PARSER_OK;
    else
        return PARSER_UNEXPECTED_TOKEN;
}
