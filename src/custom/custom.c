#include "../../inc/minishell.h"

void	custom_exec(t_cmd *cmd, t_env **env)
{
	if (ft_strncmp(cmd->cmd_arr[0], "echo", 5) == 0)
		echo_cmd(cmd);
	else if (ft_strncmp(cmd->cmd_arr[0], "cd", 3) == 0)
	{
		if (cd_cmd(cmd) == 1)
			cmd->exit_status = 1;
	}
	else if (ft_strncmp(cmd->cmd_arr[0], "pwd", 4) == 0)
	{
		if (pwd_cmd() == 1)
			cmd->exit_status = 1;
	}
	else if (ft_strncmp(cmd->cmd_arr[0], "export", 7) == 0)
	{
		if (export_cmd(cmd, env) == 1)
			cmd->exit_status = 1;
	}
	else if (ft_strncmp(cmd->cmd_arr[0], "unset", 6) == 0)
		unset_cmd(cmd, env);
	else if (ft_strncmp(cmd->cmd_arr[0], "env", 4) == 0)
	{
		if (env_cmd(cmd, *env) == 1)
			cmd->exit_status = 1;
	}
	else if (ft_strncmp(cmd->cmd_arr[0], "exit", 5) == 0)
	{
		if (exit_cmd(cmd, *env) == 1)
			cmd->exit_status = 1;
	}
}
