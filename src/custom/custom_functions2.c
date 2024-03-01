#include "../../inc/minishell.h"
#include <stdlib.h>

void    exit_cmd(t_cmd *cmd, t_env *env)
{
    int i;
    int exit_code;

    i = 0;
    exit_code = EXIT_SUCCESS;
    if (cmd->cmd_arr[1] && cmd->cmd_arr[2])
    {
        /* error ERR_ARGS*/
    }
    if (cmd->cmd_arr[1])
    {
        while (cmd->cmd_arr[1][i])
        {
            if (!ft_isdigit(cmd->cmd_arr[1][i]))
            {
                /* error ERR_ARG*/
            }
            i++;
        }
        exit_code = ft_atoi(cmd->cmd_arr[1]);
        while (exit_code < 0)
            exit_code = 256 + exit_code;
        if (exit_code > 255)
            exit_code = exit_code % 256;
    }
    clean_up(cmd, env);
    ft_putstr_fd("exit\n", 1);
    exit(exit_code);
}

