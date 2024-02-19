#include "../inc/minishell.h"

int main(int argc, char **argv)
{
    if (argc != 1 || argv[1])     
        exit(printf("This program takes no arguments\n"));
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
    {
        printf("Error: malloc failed\n");
        exit (1);
    }
    while (1)
    {
        cmd->input = readline(PROMPT);
        if (!cmd->input)
            break ;
        if (cmd->input)
            add_history(cmd->input);
        printf("You entered: %s\n", cmd->input);
        free(cmd->input);
    }
    return (0);
}
 