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
        if (strcmp(input, "exit") == 0) 
        {
            free(input);
            break;
        }
        add_history(input); // Add input to history
        free(input);
    }
    return 0;
}
