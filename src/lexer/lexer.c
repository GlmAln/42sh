#define _POSIX_C_SOURCE 200809L

#include "lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

struct status
{
    struct token *token;
    size_t size;
    char quote;
    int type;
};

static char *operators[] = {
    ";;", "&&", "||", "\n", ">>", "<<", "<>", "(", ")", "{", "}", NULL,
};

static int calloc_failed(void)
{
    fprintf(stderr, "42sh: calloc(3) has failed");
    return 1;
}

static int realloc_failed(void)
{
    fprintf(stderr, "42sh: realloc(3) has failed");
    return 1;
}

static int missing_quote(char c)
{
    fprintf(stderr, "42sh: unexpected EOF while looking for matching `%c\n", c);
    return 2;
}

static int match_operator(struct status *status, char c)
{
    int i = 0;
    char *str = status->token ? status->token->str : NULL;
    while (operators[i] != NULL)
    {
        if (strlen(operators[i]) >= status->size
            && memcmp(str, operators[i], status->size) == 0
            && operators[i][status->size] == c)
            return 1;
        i++;
    }

    return 0;
}

static int match_digits(struct status *status)
{
    size_t i = 0;
    while (i < status->size && isdigit(status->token->str[i]))
        i++;

    return i == status->size;
}

static int add_character(char c, struct status *status)
{
    if (status->token == NULL)
        if ((status->token = calloc(1, sizeof(struct token))) == NULL)
            return calloc_failed();

    struct token *token = status->token;
    if ((token->str = realloc(token->str, status->size + 1)) == NULL)
        return realloc_failed();

    token->str[c == 0 ? status->size : status->size++] = c;
    return 0;
}

static int add_token(struct lexer *lexer, struct status *status)
{
    int exit_code;
    if ((exit_code = add_character(0, status)))
        return exit_code;

    if ((lexer->list =
             realloc(lexer->list, ++lexer->size * sizeof(struct token *)))
        == NULL)
        return realloc_failed();

    status->token->type = status->type;
    lexer->list[lexer->size - 1] = status->token;

    status->token = NULL;
    status->size = 0;
    status->type = WORD;

    return 0;
}

static int rule1(char c, struct lexer *lexer, struct status *status,
                 int *exit_code)
{
    if (c == EOF)
    {
        if (status->token != NULL)
            if ((*exit_code = add_token(lexer, status)))
                return 1;

        status->type = EOF;
        *exit_code = add_token(lexer, status);
        return 1;
    }

    return 0;
}

static int rule2(char c, struct status *status, int *exit_code)
{
    if (status->type == OPERATOR && status->quote == 0
        && match_operator(status, c))
    {
        *exit_code = add_character(c, status);
        return 1;
    }
    else
        return 0;
}

static int rule3(char c, struct lexer *lexer, struct status *status,
                 int *exit_code)
{
    if (status->type == OPERATOR && status->quote == 0
        && !match_operator(status, c))
    {
        *exit_code = add_token(lexer, status);
        return 1;
    }
    else
        return 0;
}

static int rule4(char c, struct status *status, int *exit_code)
{
    if (status->quote != '\'' && c == '"')
    {
        *exit_code = add_character(c, status);
        status->quote ^= '"';
        return 1;
    }
    else if (status->quote != '"' && c == '\'')
    {
        *exit_code = add_character(c, status);
        status->quote ^= '\'';
        return 1;
    }
    else
        return 0;
}

static int rule5(char c, struct status *status, int *exit_code)
{
    if (status->quote == 0 && c == '=')
    {
        status->type = ASSIGNMENT_WORD;
        *exit_code = add_character(c, status);
        return 1;
    }
    else
        return 0;
}

static int rule6(char c, struct lexer *lexer, struct status *status,
                 int *exit_code)
{
    if (status->quote == 0)
    {
        size_t size = status->size;
        status->size = 0;

        int new = match_operator(status, c);
        status->size = size;

        if (new)
        {
            if (status->token)
            {
                if ((c == '<' || c == '>') && match_digits(status))
                    status->type = IO_NUMBER;

                if ((*exit_code = add_token(lexer, status)))
                    return 1;
            }

            *exit_code = add_character(c, status);
            status->type = OPERATOR;
            return 1;
        }
    }

    return 0;
}

static int rule7(char c, struct lexer *lexer, struct status *status,
                 int *exit_code)
{
    if (status->quote == 0 && isspace(c))
    {
        if (status->token)
            *exit_code = add_token(lexer, status);
        return 1;
    }
    else
        return 0;
}

static int rule8(char c, struct status *status, int *exit_code)
{
    if (status->size > 0
        && (status->type == WORD || status->type == ASSIGNMENT_WORD))
    {
        *exit_code = add_character(c, status);
        return 1;
    }

    return 0;
}

static int rule9(char *c, FILE *input_stream)
{
    if (*c == '#')
    {
        while (*c != EOF && *c != '\n')
            *c = fgetc(input_stream);
        if (*c == '\n')
            *c = fgetc(input_stream);
        return 1;
    }

    return 0;
}

static int rule10(char c, struct status *status, int *exit_code)
{
    *exit_code = add_character(c, status);
    status->type = WORD;
    return 1;
}

int lexer_init(FILE *input_stream, struct lexer *lexer)
{
    struct status status;
    status.token = NULL;
    status.quote = 0;
    status.size = 0;
    status.type = WORD;

    lexer->list = NULL;
    lexer->size = 0;
    lexer->curr = 0;

    int exit_code = 0;
    char c = fgetc(input_stream);
    while (!exit_code && !rule1(c, lexer, &status, &exit_code))
    {
        if (rule2(c, &status, &exit_code))
            c = fgetc(input_stream);
        else if (rule3(c, lexer, &status, &exit_code))
            continue;
        else if (rule4(c, &status, &exit_code) || rule5(c, &status, &exit_code)
                 || rule6(c, lexer, &status, &exit_code)
                 || rule7(c, lexer, &status, &exit_code)
                 || rule8(c, &status, &exit_code))
            c = fgetc(input_stream);
        else if (rule9(&c, input_stream))
            continue;
        else
        {
            rule10(c, &status, &exit_code);
            c = fgetc(input_stream);
        }
    }

    if (exit_code)
        return exit_code;

    if (status.quote)
        return missing_quote(status.quote);

    return 0;
}

struct token *lexer_peek(struct lexer *lexer)
{
    return lexer->list[lexer->curr];
}

struct token *lexer_peek2(struct lexer *lexer)
{
    return lexer->list[lexer->curr + 1];
}

struct token *lexer_pop(struct lexer *lexer)
{
    struct token *token = lexer->list[lexer->curr];

    if (lexer->curr + 1 < lexer->size)
        lexer->curr++;

    return token;
}

void lexer_free(struct lexer *lexer)
{
    for (size_t i = 0; i < lexer->size; i++)
    {
        struct token *token = lexer->list[i];
        free(token->str);
        free(token);
    }
    free(lexer->list);
}
