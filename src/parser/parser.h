#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../function/function.h"
#include "../lexer/lexer.h"

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN,
};

enum parser_status input_parse(struct ast **res, struct lexer *lexer,
                               struct function *func, int *eof);

enum parser_status list_parse(struct ast **res, struct lexer *lexer,
                              struct function *func);

enum parser_status and_or_parse(struct ast **res, struct lexer *lexer,
                                struct function *func);

enum parser_status pipeline_parse(struct ast **res, struct lexer *lexer,
                                  struct function *func);

enum parser_status command_parse(struct ast **res, struct lexer *lexer,
                                 struct function *func);

enum parser_status simple_command_parse(struct ast **res, struct lexer *lexer,
                                        struct function *func);

enum parser_status element_parse(struct ast **res, struct lexer *lexer,
                                 struct function *func);

enum parser_status shell_command_parse(struct ast **res, struct lexer *lexer,
                                       struct function *func);

enum parser_status rule_if_parse(struct ast **res, struct lexer *lexer,
                                 struct function *func);

enum parser_status else_clause_parse(struct ast **res, struct lexer *lexer,
                                     struct function *func);

enum parser_status compound_list_parse(struct ast **res, struct lexer *lexer,
                                       struct function *func);

enum parser_status redirection_parse(struct ast **res, struct lexer *lexer,
                                     struct function *func);

enum parser_status prefix_parse(struct ast **res, struct lexer *lexer,
                                struct function *func);

enum parser_status rule_while_parse(struct ast **res, struct lexer *lexer,
                                    struct function *func);

enum parser_status rule_until_parse(struct ast **res, struct lexer *lexer,
                                    struct function *func);

enum parser_status rule_for_parse(struct ast **res, struct lexer *lexer,
                                  struct function *func);

enum parser_status funcdec_parse(struct ast **res, struct lexer *lexer,
                                 struct function *func);
int is_word(struct token *tok);

int not_reserved(struct token *tok);

#endif /* ! PARSER_H */
