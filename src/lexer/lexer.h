#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <unistd.h>

enum type
{
    // EOF, // it already exists in stdio.h
    WORD,
    ASSIGNMENT_WORD,
    NAME,
    NEWLINE,
    IO_NUMBER,
    OPERATOR,
};

struct token
{
    char *str;
    int type;
};

struct lexer
{
    size_t size;
    size_t curr;
    struct token **list;
};

int lexer_init(FILE *input, struct lexer *lexer);

struct token *lexer_peek(struct lexer *lexer);
struct token *lexer_peek2(struct lexer *lexer);
struct token *lexer_pop(struct lexer *lexer);

void lexer_free(struct lexer *lexer);

#endif /*!LEXER_H*/
