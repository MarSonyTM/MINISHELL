#include "../inc/minishell.h"

int main(int argc, char **argv)
{
    if (argc != 1 || argv[1])
    {
        printf("This program takes no arguments\n");
        exit (0);
    }
    char *input;
    while (1)
    {
        input = readline(PROMPT);
        if (!input)
            break ;
        if (*input)
            add_history(input);
        printf("You entered: %s\n", input);
        free(input);
    }
    return (0);
}
 