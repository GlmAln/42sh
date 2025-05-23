#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../builtins/builtins.h"

char *charoutRes(char *str)
{
    int original_stdout = dup(fileno(stdout));

    FILE *temp_stdout = fopen("temp_stdout.txt", "w");
    cr_assert_not_null(temp_stdout, "Failed to open temp file");

    freopen("temp_stdout.txt", "w", stdout);

    // printf("%s", str);
    system(str);

    fclose(stdout);
    dup2(original_stdout, fileno(stdout));
    close(original_stdout);

    FILE *captured_output = fopen("temp_stdout.txt", "r");
    cr_assert_not_null(captured_output, "Failed to open temp file for reading");

    char *buffer = calloc(1000, sizeof(char));
    fgets(buffer, 1000, captured_output);

    fclose(captured_output);

    fflush(stdout);

    return buffer;
}

char *charout(char **str, int num)
{
    int original_stdout = dup(fileno(stdout));

    FILE *temp_stdout = fopen("temp_stdout.txt", "w");
    cr_assert_not_null(temp_stdout, "Failed to open temp file");

    freopen("temp_stdout.txt", "w", stdout);

    echo(str, num);

    fclose(stdout);
    dup2(original_stdout, fileno(stdout));
    close(original_stdout);

    FILE *captured_output = fopen("temp_stdout.txt", "r");
    cr_assert_not_null(captured_output, "Failed to open temp file for reading");

    char *buffer = calloc(1000, sizeof(char));
    fgets(buffer, 1000, captured_output);

    fclose(captured_output);

    remove("temp_stdout.txt");

    fflush(stdout);

    return buffer;
}

char *concatenate_strings(char **str, int num)
{
    if (str == NULL || num <= 0)
    {
        return NULL;
    }

    // Calculate the total length of the concatenated string
    size_t total_length = 0;
    for (int i = 0; i < num; ++i)
    {
        total_length += strlen(str[i]);
    }

    // Add space between words
    total_length += num - 1;

    // Allocate memory for the concatenated string (+1 for the null terminator)
    char *result = (char *)malloc(total_length + 1);
    if (result == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Copy each string to the result with space in between
    size_t index = 0;
    for (int i = 0; i < num; ++i)
    {
        strcpy(result + index, str[i]);
        index += strlen(str[i]);
        if (i < num - 1)
        {
            result[index++] = ' '; // Add space between words
        }
    }

    remove("temp_stdout.txt");

    return result;
}

Test(testBasics, nothing)
{
    int num = 1;
    char *str[num];
    str[0] = "echo";
    // char *str2 = concatenate_strings(str, num);
    char *buffer = charout(str, num);
    char *res = charoutRes(str[0]);
    sleep(1);
    cr_assert_str_eq(buffer, res);
}

Test(testBasics, basic)
{
    int num = 2;
    char *str[num];
    str[0] = "echo";
    str[1] = "toto";
    char *str2 = concatenate_strings(str, num);
    char *buffer = charout(str, num);
    char *res = charoutRes(str2);
    sleep(1);
    cr_assert_str_eq(buffer, res);
}

Test(testBasics, basic_n)
{
    int num = 3;
    char *str[num];
    str[0] = "echo";
    str[1] = "-n";
    str[2] = "toto";
    char *str2 = concatenate_strings(str, num);
    char *buffer = charout(str, num);
    char *res = charoutRes(str2);
    sleep(1);
    cr_assert_str_eq(buffer, res);
}

Test(testBasics, basic_e)
{
    int num = 3;
    char *str[num];
    str[0] = "echo";
    str[1] = "-n";
    str[2] = "toto\n";
    char *str2 = concatenate_strings(str, num);
    char *buffer = charout(str, num);
    char *res = charoutRes(str2);
    sleep(1);
    cr_assert_str_eq(buffer, res);
}

/*Test(testBasics, basic_E)
{
    int num = 3;
    char *str[num];
    str[0] = "echo";
    str[1] = "-E";
    str[2] = "toto\n";
    char *str2 = concatenate_strings(str, num);
    char *buffer = charout(str, num);
    char *res = charoutRes(str2);
    cr_assert_str_eq(buffer, res);
    remove("temp_stdout.txt");
}

Test(testBasics, basic_ne)
{
    int num = 3;
    char *str[num];
    str[0] = "echo";
    str[1] = "-ne";
    str[2] = "toto\\n";
    char *str2 = concatenate_strings(str, num);
    char *buffer = charout(str, num);
    char *res = charoutRes(str2);
    cr_assert_str_eq(buffer, res);
    remove("temp_stdout.txt");
}*/
