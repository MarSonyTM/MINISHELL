#include "../inc/minishell.h"

int main(int argc, char **argv) 
{
    if (argc != 1 || argv[1])
        exit(printf("This program takes no arguments\n"));

    while (1)
     {
        char *input = readline(PROMPT);
        if (!input)
            break;
        // check if user wants to exit
        if (ft_strncmp(input, "exit", 4) == 0)
        {
            printf("exit\n");
            free(input);
            break;
        }
        add_history(input); // Add input to history
        free(input);
    }
    return 0;
}
