# Init the 42sh project
AC_INIT([42sh], [1.0], [42sh@assistants.epita.fr])

# Setup automake
AM_INIT_AUTOMAKE([subdir-objects] [foreign])

# Pretty display of makefile rules
AM_SILENT_RULES([yes])

# Enable ar for Makefile
AM_PROG_AR

# Check if a ranlib is available
AC_PROG_RANLIB
CFLAGS="-O0"
# Check if a compiler is available for c
AC_PROG_CC

# Check if a compiler have this list of  flags
# AX_COMPILER_FLAGS([], [], [], [-Wall -Wextra -Werror -Wvla -pedantic -std=c99])

# List Makefile in subdirectories
AC_CONFIG_FILES([
                    Makefile
                    src/Makefile
                    src/eval_ast/Makefile
                    src/builtins/Makefile
                    src/ast/Makefile
                    src/lexer/Makefile
                    src/parser/Makefile
                    src/hash/Makefile
                    src/function/Makefile
               ])

AC_ARG_ENABLE([debug],
  AS_HELP_STRING([--enable-debug],
    [Enable debugging options (default is yes)]),
  [enable_debug=$enableval],
  [enable_debug=yes])

if test "x$enable_debug" = "xyes"; then
  AC_DEFINE([DEBUG], [1], [Enable debugging])
  CFLAGS="-g -fsanitize=address $CFLAGS"
  LDFLAGS="-fsanitize=address $LDFLAGS"
fi
AC_OUTPUT
