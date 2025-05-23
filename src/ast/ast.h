#ifndef AST_H
#define AST_H

#include <unistd.h>

enum ast_type
{
    AST_IF,
    AST_ELIF,
    AST_ELSE,
    AST_COMMAND,
    AST_PIPE,
    AST_WHILE,
    AST_UNTIL,
    AST_FOR
};

struct ast_for
{
    struct ast *body_for;
    struct token **body_in;
    char **str_in;

    size_t size_in;
    struct token *body_element;
    char *str_element;

    struct redir **redir_list;
    size_t size_redir;
};

struct ast_while
{
    struct ast *body_cond;
    struct ast *body_while;

    struct redir **redir_list;
    size_t size_redir;
};

struct ast_until
{
    struct ast *body_cond;
    struct ast *body_until;

    struct redir **redir_list;
    size_t size_redir;
};

struct ast_if
{
    struct ast *body_cond;
    struct ast *body_then;
    struct ast *body_else;

    struct redir **redir_list;
    size_t size_redir;
};

struct ast_command
{
    char **str_list;
    struct token **token_list;
    size_t size_token;

    struct token **assign_list;
    char **str_assign;
    size_t size_assign;

    struct redir **redir_list;
    size_t size_redir;
};

struct redir
{
    char *io_number;
    int and_symbole;
    int or_symbole;
    struct token *file;
    char *str_file;
    char *redirect;
};

struct ast
{
    enum ast_type type;
    struct ast *next;
    struct ast *next_and_or;
    struct ast *next_pipe;
    int negation;
    int and_or_symbole; // 1 is || ; 2 is &&

    struct ast_if *ast_if;
    struct ast_command *ast_command;
    struct ast_redirect *ast_redirect;
    struct ast_while *ast_while;
    struct ast_until *ast_until;
    struct ast_for *ast_for;
};

struct ast_if *ast_if_new(void);
struct ast_while *ast_while_new(void);
struct ast_until *ast_until_new(void);
struct ast_command *ast_command_new(void);
struct ast_for *ast_for_new(void);
struct redir *redir_new(void);
struct ast *ast_new(enum ast_type type);
void ast_free(struct ast *ast, struct ast **res);
void redir_free(struct redir *redir);
void pretty_print_ast(struct ast *t);

#endif /* !AST_H */
