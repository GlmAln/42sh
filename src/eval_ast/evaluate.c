#define _POSIX_C_SOURCE 200809L
#include "evaluate.h"

#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../ast/ast.h"
#include "../builtins/builtins.h"
#include "../hash/hash.h"
#include "../lexer/lexer.h"

static void update_table(struct hash *table, char *str)
{
    char key[1024];
    char value[1024];

    int i = 0;
    while (str[i] != 0 && str[i] != '=')
    {
        key[i] = str[i];
        i++;
    }
    key[i] = 0;

    int start = ++i;
    while (str[i] != 0)
    {
        value[i - start] = str[i];
        i++;
    }
    value[i - start] = 0;
    hash_set(table, key, value);
}

static int add_character(char **str, size_t size, char c)
{
    *str = realloc(*str, size + 1);
    if (*str == NULL)
        return 1;

    (*str)[size] = c;
    return 0;
}

static char *expand_variable(char *word, int *ref, int *save, struct hash *hash)
{
    int i = *ref;
    int j = 0;
    char c;
    while ((c = word[i + j]) != 0
           && (c == '@' || isalnum(c) || c == '*' || c == '#' || c == '?'
               || c == '$'))
        j++;

    char tmp = word[i + j];
    word[i + j] = 0;

    char *str = getenv(word + i);
    if (!str)
        str = hash_get(hash, word + i);

    str = str ? str : "";
    word[i + j] = tmp;

    i += j;
    j = 0;

    *ref = i;
    *save = j;
    return str;
}

static char *expand_word(char *word, struct hash *hash)
{
    if (!word)
        return NULL;
    char *expanded_word = NULL;
    size_t size = 0;
    char quote = 0;
    int i = 0;
    while (word[i] != '\0')
    {
        if (word[i] == '\\')
        {
            i++;
            if (quote == '"')
            {
                if (word[i] != '$' && word[i] != '`' && word[i] != '\\'
                    && word[i])
                    add_character(&expanded_word, size++, '\\');
                add_character(&expanded_word, size++, word[i++]);
            }
            else if (quote == '\'')
            {
                add_character(&expanded_word, size++, '\\');
                add_character(&expanded_word, size++, word[i++]);
            }
            else if (word[i] != 0)
                add_character(&expanded_word, size++, word[i++]);
            else
                add_character(&expanded_word, size++, '\\');
        }
        else if (word[i] == '\'')
        {
            quote ^= '\'';
            i++;
        }
        else if (word[i] == '"')
        {
            quote ^= '"';
            i++;
        }
        else if (quote != '\'' && word[i] == '$')
        {
            i++;
            int j;
            char *str = expand_variable(word, &i, &j, hash);
            i += j;
            while (str[j])
                add_character(&expanded_word, size++, str[j++]);
        }
        else
            add_character(&expanded_word, size++, word[i++]);
    }
    add_character(&expanded_word, size++, 0);

    return expanded_word;
}

static void expansion_assign(struct ast_command *t, struct hash **table)
{
    for (size_t i = 0; i < t->size_assign; i++)
    {
        char *tmp = expand_word(t->str_assign[i], *table);
        free(t->str_assign[i]);
        t->str_assign[i] = tmp;
    }
}

static void expansion_command(struct ast_command *t, struct hash **table)
{
    for (size_t i = 0; i < t->size_token; i++)
    {
        char *tmp = expand_word(t->token_list[i]->str, *table);
        free(t->str_list[i]);
        t->str_list[i] = tmp;
    }
    expansion_assign(t, table);
}

static void expansion_redirect(struct redir *t, struct hash **table)
{
    char *tmp = expand_word(t->file->str, *table);
    free(t->str_file);
    t->str_file = tmp;
}

static void expansion_for(struct ast_for *t, struct hash **table)
{
    char *tmp = expand_word(t->body_element->str, *table);
    free(t->str_element);
    t->str_element = tmp;

    for (size_t i = 0; i < t->size_in; i++)
    {
        char *tmp2 = expand_word(t->body_in[i]->str, *table);
        free(t->str_in[i]);
        t->str_in[i] = tmp2;
    }
}

static void str_revert(char *str, size_t len)
{
    char tmp;
    for (size_t i = 0; i < len / 2; i++)
    {
        tmp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = tmp;
    }
}

static char *my_itoa(int value, char *s)
{
    if (value == 0)
    {
        s[0] = '0';
        s[1] = '\0';
        return s;
    }
    s[0] = '\0';
    size_t i = 1;
    int neg = 0;
    if (value < 0)
    {
        neg = 1;
        value = -value;
    }
    int res;
    while (value > 0)
    {
        res = value % 10;
        value = value / 10;
        char c = '0' + res;
        s[i] = c;
        i += 1;
    }
    if (neg == 1)
    {
        s[i] = '-';
        i += 1;
    }
    str_revert(s, i);
    return s;
}

int executeScript(int j, char **argv)
{
    int i = j + 1;
    while (argv[i])
    {
        i++;
        char *tmp = malloc(2);
        setenv(my_itoa(i - 1, tmp), argv[i - 1], 0);
        free(tmp);
    }
    pid_t pid = fork();

    if (pid == -1)
        return 1;

    if (pid == 0)
    {
        char buff[1024];
        char *tmp = getcwd(buff, 1024);
        strcpy(tmp + strlen(tmp), "/");
        strcpy(tmp + strlen(tmp), argv[j]);

        execlp("/bin/sh", "/bin/sh", "-c", tmp, (char *)NULL);
        return 1;
    }
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        return 0;
    return 1;
}

static void update_env(char *str)
{
    char key[1024];
    char value[1024];

    int i = 0;
    while (str[i] != 0 && str[i] != '=')
    {
        key[i] = str[i];
        i++;
    }
    key[i] = 0;

    int start = ++i;
    while (str[i] != 0)
    {
        value[i - start] = str[i];
        i++;
    }
    value[i - start] = 0;
    setenv(key, value, 1);
}

static void update_env_all(struct ast_command *t)
{
    for (size_t i = 0; i < t->size_assign; i++)
        update_env(t->str_assign[i]);
}

static int myexec(char **argv, struct ast_command *t)
{
    int error = 0;
    int status;

    if (!argv[1] && access(argv[0], F_OK) == 0)
    {
        return executeScript(0, argv);
    }

    int pid = fork();

    if (pid < 0)
        err(1, "fork() failed\n");

    if (pid == 0)
    {
        update_env_all(t);
        execvp(argv[0], argv);
        if (errno == ENOENT)
        {
            fprintf(stderr, "42sh: %s: command not found\n", *argv);
            return 127;
        }
    }
    else
    {
        waitpid(pid, &status, 0);
        error = WEXITSTATUS(status);
        if (error == ENOENT)
            err(127, "command not existing...\n");
    }
    return error;
}

static void redirect_stdout(struct redir *t)
{
    int flags = O_WRONLY | O_CREAT;
    if (strcmp(t->redirect, ">") == 0)
    {
        flags |= O_TRUNC; // erase data
    }
    else
        flags |= O_APPEND; // append data

    int fd = open(t->str_file, flags, 0644);

    if (fd == -1) // failed to open the file
        err(1, "open() error...\n");

    int stdout_fd = STDOUT_FILENO;

    if (t->io_number)
    {
        int io_number = atoi(t->io_number);
        if (io_number >= 0)
            stdout_fd = io_number;
    }

    if (t->and_symbole == 1)
    {
        if (dup2(fd, STDERR_FILENO) == -1)
            err(1, "dup2() failed...\n");
    }
    if (dup2(fd, stdout_fd) == -1)
        err(1, "dup2() failed...\n");

    close(fd);
}

static void redirect_stdin(struct redir *t)
{
    int flags = O_RDONLY;

    int fd = open(t->str_file, flags, 0644);

    if (fd == -1) // failed to open the file
    {
        fprintf(stderr, "file not existing\n");
        exit(1);
    }

    int stdin_fd = STDIN_FILENO;

    if (t->io_number)
    {
        int io_number = atoi(t->io_number);
        if (io_number >= 0)
            stdin_fd = io_number;
    }

    if (t->and_symbole == 1)
    {
        if (dup2(fd, STDERR_FILENO) == -1)
            err(1, "dup2() failed...\n");
    }
    if (dup2(fd, stdin_fd) == -1)
        err(1, "dup2() failed...\n");

    close(fd);
}

static void redirect_stdin_out(struct redir *t)
{
    int flags = O_RDWR | O_CREAT;
    int fd = open(t->str_file, flags, 0644);

    if (fd == -1)
    {
        perror("open() error");
        exit(EXIT_FAILURE);
    }

    int stdin_fd = STDIN_FILENO;
    int stdout_fd = STDOUT_FILENO;

    if (t->io_number)
    {
        int io_number = atoi(t->io_number);
        if (io_number >= 0)
        {
            stdin_fd = io_number;
            stdout_fd = io_number;
        }
    }

    if (t->and_symbole == 1)
    {
        if (dup2(fd, STDERR_FILENO) == -1)
        {
            perror("dup2() for STDERR failed");
            exit(EXIT_FAILURE);
        }
    }

    if (dup2(fd, stdin_fd) == -1)
    {
        perror("dup2() for STDIN failed");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, stdout_fd) == -1)
    {
        perror("dup2() for STDOUT failed");
        exit(EXIT_FAILURE);
    }

    // Redirect standard input and output to the file

    /*if (dup2(STDOUT_FILENO, STDOUT_FILENO) == -1)
      {
      perror("dup2() for STDOUT failed");
      exit(EXIT_FAILURE);
      }*/

    close(fd);
}

static int exec_pipe(struct ast *t1, struct ast *t2, struct hash **hash,
                     struct function *func)
{
    int pipefds[2];
    if (pipe(pipefds) == -1)
    {
        perror("pipe() failed");
        exit(EXIT_FAILURE);
    }

    int pid = fork();

    if (pid == -1)
    {
        perror("fork() failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        // Close the reading end of the pipe in the child process
        close(pipefds[0]);

        if (dup2(pipefds[1], STDOUT_FILENO) == -1)
        {
            perror("dup2() 1 failed");
            exit(EXIT_FAILURE);
        }
        close(pipefds[1]);

        expansion_command(t1->ast_command, hash);
        int ex = execCommand(t1->ast_command, hash, func);

        exit(ex);
    }
    else
    {
        // Close the writing end of the pipe in the parent process
        close(pipefds[1]);

        if (dup2(pipefds[0], STDIN_FILENO) == -1)
        {
            perror("dup2() 2 failed");
            exit(EXIT_FAILURE);
        }
        close(pipefds[0]);

        int status;
        waitpid(pid, &status, 0);
        // return WEXITSTATUS(status);
        expansion_command(t2->ast_command, hash);
        int res = execCommand(t2->ast_command, hash, func);

        if (t1->negation)
        {
            res = res > 0 ? 0 : 1;
        }
        return res;
    }
}

static void select_redirect(struct redir *t, struct hash **hash)
{
    expansion_redirect(t, hash);
    if (t->redirect[0] == '>' || t->redirect[1] == '>')
        redirect_stdout(t);
    else if (t->redirect[0] == '<')
        redirect_stdin(t);
    else if (t->redirect[0] == '<' && t->redirect[1] == '>')
        redirect_stdin_out(t);
}

static int execEcho(char **args, size_t size, struct hash **hash_map)
{
    if (hash_map == NULL)
    {
        echo(args, size);
        return True();
    }
    echo(args, size);
    return True();
}

static int execTrue(char **args, size_t size, struct hash **hash_map)
{
    if (size == 0 || args == NULL || hash_map == NULL)
        return True();
    return True();
}

static int execFalse(char **args, size_t size, struct hash **hash_map)
{
    if (size == 0 || args == NULL || hash_map == NULL)
        return False();
    return False();
}

static int execCd(char **args, size_t size, struct hash **hash_map)
{
    return builtins_cd(size, args, hash_map);
}

static int execContinue(char **args, size_t size, struct hash **hash_map)
{
    if (size == 0 || args == NULL || hash_map == NULL)
        return builtins_continue();
    return builtins_continue();
}

static int execBreak(char **args, size_t size, struct hash **hash_map)
{
    if (size == 0 || args == NULL || hash_map == NULL)
        return builtins_break();
    return builtins_break();
}

static int execExport(struct ast_command *t, struct hash **hash_map)
{
    return builtins_export(t, hash_map);
}

static int execExit(char **args, size_t size, struct hash **hash_map)
{
    if (size == 0 || args == NULL || hash_map == NULL)
        return builtins_exit(size, args);
    return builtins_exit(size, args);
}

char **create_args_splitted(struct token **list, char **str_list, size_t *size)
{
    size_t size_args = 0;
    char **args = NULL;

    for (size_t i = 0; i < *size; i++)
    {
        if (!strcmp(list[i]->str, "$@") || !strcmp(list[i]->str, "$*"))
        {
            size_t ind = 0;
            while (str_list[i][ind] != 0)
            {
                size_t j = 0;
                char *value = malloc(sizeof(char));
                while (str_list[i][ind + j] != 0 && str_list[i][ind + j] != ' ')
                {
                    value = realloc(value, sizeof(char) * (j + 2));
                    value[j] = str_list[i][ind + j];
                    j++;
                }

                value[j] = 0;
                args = realloc(args, sizeof(char *) * ++size_args);
                args[size_args - 1] = value;

                if (str_list[i][ind + j] == 0)
                    break;

                ind += j + 1;
                while (isblank(str_list[i][ind]) && str_list[i][ind] != '0')
                    ind++;
            }
        }
        else
        {
            args = realloc(args, sizeof(char *) * ++size_args);
            char *tmp = calloc(strlen(str_list[i]) + 1, sizeof(char));
            strcpy(tmp, str_list[i]);
            args[size_args - 1] = tmp;
        }
    }
    *size = size_args;
    return args;
}

static void free_args(char **args, size_t size_args)
{
    for (size_t i = 0; i < size_args; i++)
        free(args[i]);
    free(args);
}

static int execDot(char **args, size_t size, struct hash **hash_map,
                   struct function *func)
{
    if (size == 0 || args == NULL || hash_map == NULL)
        return builtins_dot(args[1], hash_map, func);
    return builtins_dot(args[1], hash_map, func);
}

static int execUnset(char **args, size_t size, struct hash **hash_map,
                     struct function *func)
{
    if (size == 0 || args == NULL || hash_map == NULL)
        return builtins_unset(size, args, hash_map, func);
    return builtins_unset(size, args, hash_map, func);
}

int execCommand(struct ast_command *t, struct hash **hash,
                struct function *func)
{
    if (t->size_token == 0)
    {
        for (size_t i = 0; i < t->size_assign; i++)
            update_table(*hash, t->assign_list[i]->str);
        return 0;
    }

    size_t size_args = t->size_token;
    char **args = create_args_splitted(t->token_list, t->str_list, &size_args);

    if (strcmp(args[0], ".") == 0)
    {
        int res = execDot(args, size_args, hash, func);
        free_args(args, size_args);
        return res;
    }
    else if (strcmp(args[0], "export") == 0)
    {
        int res = execExport(t, hash);
        free_args(args, size_args);
        return res;
    }
    else if (function_get(func, args[0]) != NULL)
    {
        int res = evaluate(function_get(func, args[0]), hash, func);
        free_args(args, size_args);
        return res;
    }
    else if (strcmp(args[0], "unset") == 0)
    {
        int res = execUnset(args, size_args, hash, func);
        free_args(args, size_args);
        return res;
    }
    CommandFunction commandFunctions[] = { execEcho, execTrue,     execFalse,
                                           execCd,   execContinue, execBreak,
                                           execExit };

    const char *commandNames[] = { "echo",     "true",  "false", "cd",
                                   "continue", "break", "exit" };

    for (size_t i = 0;
         i < sizeof(commandFunctions) / sizeof(commandFunctions[0]); i++)
    {
        if (!strcmp(args[0], commandNames[i]))
        {
            int res = commandFunctions[i](args, size_args, hash);
            free_args(args, size_args);
            return res;
        }
    }

    args = realloc(args, sizeof(char *) * ++size_args);
    args[size_args - 1] = NULL;

    int error = myexec(args, t);
    free_args(args, size_args);
    return error;
}

static int evIF(struct ast *t, struct hash **hash, struct function *func)
{
    int res = 0;
    int save_stdout = dup(STDOUT_FILENO);
    int save_stderr = dup(STDERR_FILENO);
    int save_stdin = dup(STDIN_FILENO);

    for (size_t i = 0; i < t->ast_if->size_redir; i++)
    {
        select_redirect(t->ast_if->redir_list[i], hash);
    }

    int condition = evaluate(t->ast_if->body_cond, hash, func);
    if (!condition)
        res = evaluate(t->ast_if->body_then, hash, func);
    else
        res = evaluate(t->ast_if->body_else, hash, func);

    dup2(save_stdout, STDOUT_FILENO);
    dup2(save_stderr, STDERR_FILENO);
    dup2(save_stdin, STDIN_FILENO);
    close(save_stdout);
    close(save_stderr);
    close(save_stdin);
    return res;
}

static int evCOMMAND(struct ast *t, struct hash **hash, struct function *func)
{
    int res = 0;
    expansion_command(t->ast_command, hash);

    int save_stdout = dup(STDOUT_FILENO);
    int save_stderr = dup(STDERR_FILENO);
    int save_stdin = dup(STDIN_FILENO);

    for (size_t i = 0; i < t->ast_command->size_redir; i++)
    {
        select_redirect(t->ast_command->redir_list[i], hash);
    }

    res = execCommand(t->ast_command, hash, func);
    res = t->negation == 1 ? !res : res;
    if (t->next_and_or != NULL)
    {
        if (t->next_and_or->and_or_symbole == 1 && res == 0) // is &&
            return evaluate(t->next_and_or, hash, func);
        else if (t->next_and_or->and_or_symbole == 2 && res != 0) // is ||
            return evaluate(t->next_and_or, hash, func);
    }

    dup2(save_stdout, STDOUT_FILENO);
    dup2(save_stderr, STDERR_FILENO);
    dup2(save_stdin, STDIN_FILENO);
    close(save_stdout);
    close(save_stderr);
    close(save_stdin);
    return res;
}

static int evWHILE(struct ast *t, struct hash **hash, struct function *func)
{
    int res = 0;
    int save_stdout = dup(STDOUT_FILENO);
    int save_stderr = dup(STDERR_FILENO);
    int save_stdin = dup(STDIN_FILENO);

    for (size_t i = 0; i < t->ast_while->size_redir; i++)
    {
        select_redirect(t->ast_while->redir_list[i], hash);
    }

    while (evaluate(t->ast_while->body_cond, hash, func) == 0)
    {
        res = evaluate(t->ast_while->body_while, hash, func);
        if (res == -2) // continue
            continue;
        if (res == -4) // break
            break;
        if (res < 0)
            return res;
    }
    dup2(save_stdout, STDOUT_FILENO);
    dup2(save_stderr, STDERR_FILENO);
    dup2(save_stdin, STDIN_FILENO);
    close(save_stdout);
    close(save_stderr);
    close(save_stdin);
    return 0;
}

static int evUNTIL(struct ast *t, struct hash **hash, struct function *func)
{
    int res = 0;
    int save_stdout = dup(STDOUT_FILENO);
    int save_stderr = dup(STDERR_FILENO);
    int save_stdin = dup(STDIN_FILENO);

    for (size_t i = 0; i < t->ast_until->size_redir; i++)
    {
        select_redirect(t->ast_until->redir_list[i], hash);
    }

    while (evaluate(t->ast_until->body_cond, hash, func) != 0)
    {
        res = evaluate(t->ast_until->body_until, hash, func);
        if (res == -2) // continue
            continue;
        if (res == -4) // break
            break;
        if (res < 0)
            return res;
    }

    dup2(save_stdout, STDOUT_FILENO);
    dup2(save_stderr, STDERR_FILENO);
    dup2(save_stdin, STDIN_FILENO);
    close(save_stdout);
    close(save_stderr);
    close(save_stdin);
    return 0;
}

static int evPIPE(struct ast *t, struct hash **hash, struct function *func)
{
    int res = exec_pipe(t, t->next_pipe, hash, func);

    if (t->next_and_or != NULL)
    {
        if (t->and_or_symbole == 2 && res == 0) // is &&
            return evaluate(t->next_and_or, hash, func);
        else if (t->and_or_symbole == 1 && res != 0) // is ||
            return evaluate(t->next_and_or, hash, func);
    }
    return res;
}

static int evFOR(struct ast *t, struct hash **hash, struct function *func)
{
    expansion_for(t->ast_for, hash);
    int res = 0;

    size_t size_args = t->ast_for->size_in;
    char **args = create_args_splitted(t->ast_for->body_in, t->ast_for->str_in,
                                       &size_args);

    int save_stdout = dup(STDOUT_FILENO);
    int save_stderr = dup(STDERR_FILENO);
    int save_stdin = dup(STDIN_FILENO);
    for (size_t i = 0; i < t->ast_for->size_redir; i++)
    {
        select_redirect(t->ast_for->redir_list[i], hash);
    }

    size_t k = 0;
    while (k < size_args)
    {
        setenv(t->ast_for->str_element, args[k], 1);
        hash_set(*hash, t->ast_for->str_element, args[k]);
        res = evaluate(t->ast_for->body_for, hash, func);
        k++;
        // printf("%i\n", res);
        if (res == -2) // continue
            continue;
        if (res == -4) // break
            break;
        if (res < 0)
            break;
    }

    setenv(t->ast_for->str_element, "", 1);
    char *nul = calloc(1, sizeof(char));
    hash_set(*hash, t->ast_for->str_element, nul);
    free(nul);
    for (size_t i = 0; i < size_args - 0; i++)
        free(args[i]);
    free(args);

    dup2(save_stdout, STDOUT_FILENO);
    dup2(save_stderr, STDERR_FILENO);
    dup2(save_stdin, STDIN_FILENO);
    close(save_stdout);
    close(save_stderr);
    close(save_stdin);

    return res;
}

int evaluate(struct ast *t, struct hash **hash, struct function *func)
{
    if (t == NULL)
        return False();

    int res = 0;

    if (t->type == AST_IF || t->type == AST_ELIF)
    {
        res = evIF(t, hash, func);
    }

    else if (t->type == AST_COMMAND)
    {
        if (t->ast_command != NULL)
        {
            res = evCOMMAND(t, hash, func);
        }
    }

    else if (t->type == AST_ELSE)
    {
        res = evaluate(t->ast_if->body_then, hash, func);
    }

    else if (t->type == AST_WHILE)
    {
        res = evWHILE(t, hash, func);
    }

    else if (t->type == AST_UNTIL)
    {
        res = evUNTIL(t, hash, func);
    }

    else if (t->type == AST_FOR)
    {
        res = evFOR(t, hash, func);
    }
    else if (t->type == AST_PIPE)
    {
        res = evPIPE(t, hash, func);
    }

    if (res == -2 || res == -4)
        return res;

    if (t->next)
    {
        res = evaluate(t->next, hash, func);
        if (res == -4 || res == -2)
            return res;
        t = t->next;
    }
    return res;
}
