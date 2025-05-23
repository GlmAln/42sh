# define the sub directories
SUBDIRS = hash lexer eval_ast parser ast builtins function

bin_PROGRAMS = 42sh

42sh_SOURCES = 42sh.c

42sh_CPPFLAGS = -I%D%

42sh_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

42sh_LDADD = hash/libhash.a lexer/liblexer.a ast/libast.a parser/libparser.a builtins/libbuiltins.a eval_ast/libeval_ast.a function/libfunction.a
