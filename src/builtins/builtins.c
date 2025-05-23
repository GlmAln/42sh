#define _POSIX_C_SOURCE 200809L
#include "builtins.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../eval_ast/evaluate.h"
#include "../hash/hash.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"

// take a string and tranform all \\n into \n
static void escape_str(char **str)
{
    int i = 0;
    int j = 0;
    while ((*str)[i])
    {
        if ((*str)[i] == '\\' && (*str)[i + 1])
        {
            switch ((*str)[i + 1])
            {
            case 'n':
                (*str)[j] = '\n';
                break;
            case 't':
                (*str)[j] = '\t';
                break;
            default:
                (*str)[j] = (*str)[i];
                break;
            }
            i += 2;
        }
        else
        {
            (*str)[j] = (*str)[i];
            i++;
        }
        j++;
    }
    (*str)[j] = 0;
}

// check if argument n e or E are here in str
static int is_echo_variable(char **str, int j)
{
    char c = str[j][1];
    if (c == 'n' || c == 'e' || c == 'E')
        return 1;
    return 0;
}

void echo(char **str, int num)
{
    if (num == 1)
    {
        printf("\n");
        return;
    }

    int newline = 1;
    int escape = 0;

    int j = 1;
    int stopParam = 0;
    int arrLen = 0;
    int *arr = calloc(256, sizeof(int));
    while (j < num)
    {
        int len = strlen(str[j]);

        if (str[j][0] != '-' || stopParam == 1
            || (str[j][0] == '-' && !str[j][1]) || !is_echo_variable(str, j))
        {
            stopParam = 1;
            arr[arrLen++] = j;
            j++;
            continue;
        }

        for (int i = 1; i < len; i++)
        {
            if (str[j][i] == 'n')
            {
                newline = 0;
            }
            if (str[j][i] == 'e')
            {
                escape = 1;
            }
            if (str[j][i] == 'E')
            {
                escape = 0;
            }
        }
        j++;
    }

    if (escape == 1)
    {
        for (int i = 0; i < arrLen; i++)
            escape_str(&(str[arr[i]]));
    }

    for (int i = 0; i < arrLen - 1; i++)
    {
        printf("%s ", str[arr[i]]);
    }
    printf("%s", str[arr[arrLen - 1]]);

    if (newline)
        printf("\n");

    fflush(stdout);
    free(arr);
}

int True(void)
{
    return 0;
}

int False(void)
{
    return 1;
}

int builtins_continue(void)
{
    return -2;
}

int builtins_break(void)
{
    return -4;
}

/*
Assume a given char **args such as 'cd target'
therefore args[0] would be 'cd'
and 'target' would be args[1]
*/

static char *get_next(char *curent, char *dest, char *delim)
{
    if (!curent || strcmp(curent, "") == 0)
        return NULL;
    size_t i = 0;
    if (curent[i] == '/')
        i++;
    while (curent[i] != '\0' && curent[i] != delim[0])
        i++;
    strncpy(dest, curent, i);
    dest[i] = '\0';

    return dest;
}

static char *cd_rule_6(char *directory, char *current_path)
{
    strcpy(current_path, directory);
    return current_path;
}
static char *cd_rule_7(char *current_path)
{
    if (current_path[0] != '\0' && current_path[0] != '/')
    {
        current_path = realloc(current_path, 1024);
        if (current_path == NULL)
            return current_path;
        char *tmp = strdup(current_path);
        char pwd[1024];
        strcpy(current_path, getcwd(pwd, 1024));

        strcat(current_path, "/");
        strcat(current_path, tmp);
        free(tmp);
    }
    return current_path;
}

static char *cd_rule_5(char *component, char *current_path)
{
    char *cdpath = getenv("CDPATH");
    if (cdpath != NULL)
    {
        char *path = malloc(1024);
        path = get_next(cdpath, path, ":");
        if (path == NULL)
            path = ".";
        int len = strlen(path) + strlen(component);
        if (path[strlen(path) - 1] == '/')
            len++;
        char *test_dir = malloc(sizeof(char) * len);
        strcat(test_dir, path);
        if (path[strlen(path) - 1] != '/')
            strcat(test_dir, "/");
        strcat(test_dir, component);
        while (access(test_dir, X_OK) != 0)
        {
            if (strcmp(path, ".") == 0)
                break;
            path = strtok(NULL, ":");
            if (path == NULL)
                path = ".";
            len = strlen(path) + strlen(component);
            if (path[strlen(path) - 1] == '/')
                len++;
            test_dir = realloc(test_dir, sizeof(char) * len);
            strcat(test_dir, path);
            if (path[strlen(path) - 1] != '/')
                strcat(test_dir, "/");
            strcat(test_dir, component);
        }
        if (access(test_dir, X_OK) == 0)
        {
            free(current_path);
            return cd_rule_7(test_dir);
        }
        return current_path;
    }
    return component;
}

static void cd_rule_8(char *curentpath)
{
    if (curentpath == NULL)
        return;
    char *res = strdup(curentpath);
    char *ressave = res;
    char *component = calloc(1, 1024);
    char *compsave = component;
    component = get_next(res, component, "/");
    char *prec = strdup(curentpath);
    while (component != NULL && strcmp(component, "") != 0)
    {
        if (strcmp(component, "/.") != 0)
        {
            if (strcmp(component, "/..") == 0)
            {
                if (strcmp(prec, "/..") != 0)
                {
                    res -= strlen(prec);
                    *(res) = '\0';
                }
                else if (res > curentpath)
                {
                    res = strrchr(curentpath, '/');
                    if (res == NULL)
                        res = curentpath;
                }
            }
            else
            {
                if (res > curentpath)
                    *res = '/';
                strncpy(res, component, strlen(component));
                res += strlen(component);
            }
        }
        strcpy(prec, component);
        component = get_next(res, component, "/");
    }
    *res = '\0';
    curentpath[0] = '/';
    free(compsave);
    free(prec);
    strcpy(curentpath, ressave);
    free(ressave);
}

static char *cd_get_new_dir(char *directory)
{
    char *current_path = calloc(1, sizeof(char) * 1024);
    if (strcmp(directory, "-") == 0)
    {
        char *res = getenv("OLDPWD");
        free(current_path);

        if (res == NULL)
            return NULL;
        return res;
    }
    char *component = malloc(1024);
    component = get_next(directory, component, "/");
    if (directory[0] == '/')
    {
        strcpy(current_path, directory);
        current_path = cd_rule_7(current_path);
    }
    else if (strcmp(component, ".") == 0 || strcmp(component, "..") == 0)
    {
        current_path = cd_rule_6(directory, current_path);
        current_path = cd_rule_7(current_path);
    }
    else
    {
        char *return_val = cd_rule_5(component, current_path);
        if (strcmp(return_val, directory) == 0)

            current_path = strncpy(current_path, directory, 1023);
        current_path = cd_rule_6(directory, current_path);
        current_path = cd_rule_7(current_path);
    }
    cd_rule_8(current_path);
    free(component);
    return current_path;
}

static int change_directory(char *new_dir, char *old_dir,
                            struct hash **hash_map, int argc)
{
    int res = chdir(new_dir);
    if (res == -1 && old_dir && hash_map)
    {
        fprintf(stderr, "command cd: %s: No such file or direcotry\n", new_dir);
        free(new_dir);
        return 1;
    }
    hash_set(*hash_map, "OLDPWD", old_dir);
    hash_set(*hash_map, "PWD", new_dir);
    setenv("PWD", new_dir, 1);
    setenv("OLDWPD", old_dir, 1);
    if (argc != 1)
        free(new_dir);

    return 0;
}

int builtins_cd(int argc, char **argv, struct hash **hash_map)
{
    char buff[1024];
    if (argc == 1)
    {
        char *home_env = getenv("HOME");
        if (home_env == NULL || strcmp(home_env, "") == 0)
        {
            fprintf(stderr, "command cd: HOME not set\n");
            return 1;
        }
        return change_directory(home_env, getcwd(buff, 1024), hash_map, argc);
    }
    char *new_dir = cd_get_new_dir(argv[1]);
    if (!new_dir)
    {
        fprintf(stderr, "command cd: OLDPWD or HOME not set\n");

        return 1;
    }
    return change_directory(new_dir, getcwd(buff, 1024), hash_map, argc);
}

/*
This builtins_export only work with export name[=word] and export NAME will be
test export alone normaly give all export variable but not necessary here
*/
int builtins_export(struct ast_command *t, struct hash **hash)
{
    if (t->size_token == 1 && t->size_assign == 0)
    {
        printf("export usage: export name[=word]\n");
        return 0;
    }
    char *var_name;
    if (t->size_token == 2)
        var_name = calloc(1, strlen(t->str_list[1]) + 1);
    else
        var_name = calloc(1, strlen(t->assign_list[0]->str) + 1);
    if (t->assign_list)
    {
        int i = 0;
        while (t->assign_list[0]->str[i] != '\0'
               && t->assign_list[0]->str[i] != '=')
            i++;
        strncpy(var_name, t->assign_list[0]->str, i);
        var_name[i] = '\0';
    }
    else
    {
        strcpy(var_name, t->str_list[1]);
        var_name[strlen(t->str_list[1])] = '\0';
    }
    char *var_value;
    if (t->assign_list)
        var_value = strchr(t->assign_list[0]->str, '=') + 1;
    else
        var_value = "";
    hash_set(*hash, var_name, var_value);
    int res = setenv(var_name, var_value, 1);
    free(var_name);
    return res;
}

int builtins_unset(int argc, char **argv, struct hash **hash_map,
                   struct function *func)
{
    if (argc == 1)
        return 0;
    int v = 0;
    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "-vf"))
        v = 1;
    if (v == 1 && argc != 3)
    {
        fprintf(stderr, "42sh: unset name\n");
        return -1;
    }
    if (v == 1 && argv[2])
    {
        function_set(func, argv[2], NULL);
        hash_set(*hash_map, argv[2], NULL);
        unsetenv(argv[2]);
    }
    return 0;
}

static size_t is_number(char *arg)
{
    size_t i = 0;
    while (i < strlen(arg))
    {
        if (!isdigit(arg[i]))
            break;
        i++;
    }
    return (i == strlen(arg));
}

int builtins_exit(int argc, char **argv)
{
    int exit_code = -1024;
    printf("exit\n");
    if (argc == 1)
        return exit_code;
    else if (argc >= 3)
        printf("42sh: exit: too many arguments\n");
    else if (argc == 2 && is_number(argv[1]) == 1)
        return exit_code + (atoi(argv[1]) % 256);
    else if (argc == 2)
        fprintf(stderr, "42sh: exit: %s: numeric argument required\n", argv[1]);
    return exit_code + 1;
}

int builtins_dot(char *str, struct hash **hash_map, struct function *func)
{
    size_t i = strlen(str);
    if (str[i - 1] != 'h' || str[i - 2] != 's' || str[i - 3] != '.'
        || access(str, F_OK) == -1)
    {
        fprintf(stderr, "42sh: file not found\n");
        return 2;
    }
    FILE *fp = fopen(str, "r");

    int exit_code = 0;
    struct lexer lexer;
    if ((exit_code = lexer_init(fp, &lexer)))
        return exit_code;

    struct ast *res = NULL;
    int eof = 0;
    int resParse;
    while (!eof && exit_code >= 0
           && (resParse = input_parse(&res, &lexer, func, &eof)) == PARSER_OK)
    {
        exit_code = evaluate(res, hash_map, func);
        ast_free(res, NULL);
    }

    exit_code = resParse == PARSER_OK ? exit_code : 2;

    /*struct ast *res = NULL;
    if (input_parse(&res, &lexer) == PARSER_OK)
    {
        exit_code = evaluate(res, hash_map);
        ast_free(res, NULL);
    }
    else
        exit_code = 2;*/

    lexer_free(&lexer);
    fclose(fp);

    return exit_code < -512 ? exit_code + 1024 : exit_code;
}
