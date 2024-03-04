#include "../../inc/minishell.h"

void    echo_cmd(t_cmd *cmd)
{
    int i;
    int newline;

    newline = 1;
    i = 1;
    if (cmd->cmd_arr[1] && ft_strncmp(cmd->cmd_arr[1], "-n", 3) == 0)
    {
        newline = 0;
        i++;
    }
    while (cmd->cmd_arr[i] != NULL)
    {
        ft_putstr_fd(cmd->cmd_arr[i], 1);
        if (cmd->cmd_arr[i + 1] != NULL)
            ft_putchar_fd(' ', 1);
        i++;
    }
    if (newline == 1)
        ft_putchar_fd('\n', 1);
}

void    cd_cmd(t_cmd *cmd)
{
    if (cmd->cmd_arr[1] == NULL)
    {
        if (chdir(getenv("HOME")) == -1)
        {
            /* error */
        }
    }
    else if (chdir(cmd->cmd_arr[1]) == -1)
    {
        /* error */
    }
}

void    pwd_cmd(void)
{
    char *pwd;

    pwd = getcwd(NULL, 0);
    if (pwd == NULL)
    {
        /* error */
    }
    ft_putstr_fd(pwd, 1);
    free(pwd);
}

void    env_cmd(t_cmd *cmd, t_env *env)
{
    t_env *tmp;

    if (cmd->cmd_arr[1] != NULL)
    {
        /* error */
    }
    tmp = env;
    while (tmp != NULL)
    {
        ft_putstr_fd(tmp->key, 1);
        ft_putchar_fd('=', 1);
        ft_putendl_fd(tmp->value, 1);
        tmp = tmp->next;
    }
}

void    unset_cmd(t_cmd *cmd, t_env **env)
{
    t_env   *tmp;
    t_env   *prev;
    int     i;

    i = 1;
    while (cmd->cmd_arr[i] != NULL)
    {
        tmp = *env;
        prev = NULL;
        while (tmp != NULL)
        {
            if (ft_strncmp(tmp->key, cmd->cmd_arr[i], ft_strlen(cmd->cmd_arr[i]) + 1) == 0)
            {
                if (prev == NULL)
                    *env = tmp->next;
                else
                    prev->next = tmp->next;
                free(tmp->key);
                free(tmp->value);
                free(tmp);
                break ;
            }
            prev = tmp;
            tmp = tmp->next;
        }
        i++;
    }
}
