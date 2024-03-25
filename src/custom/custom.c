#include "../../inc/minishell.h"

int	export_cmd(t_cmd *cmd, t_env **env)
{
	int	i;

	i = 1;
	if (!cmd->cmd_arr[1])
	{
		env_cmd(cmd, *env);
		return (1);
	}
	if (cmd->cmd_arr[1][0] == '=' || ft_isdigit(cmd->cmd_arr[1][0]))
	{
		error(ERR_VAL, "export", cmd->cmd_arr[1], 1);
		return (1);
	}
	while (cmd->cmd_arr[i])
	{
		handle_export_args(cmd, env, i);
		i++;
	}
	return (0);
}

void	custom_exec(t_cmd *cmd, t_env **env)
{
	int	exit_status;

	exit_status = 0;
	if (ft_strncmp(cmd->cmd_arr[0], "echo", 5) == 0)
		echo_cmd(cmd);
	else if (ft_strncmp(cmd->cmd_arr[0], "cd", 3) == 0)
		exit_status = cd_cmd(cmd, *env);
	else if (ft_strncmp(cmd->cmd_arr[0], "pwd", 4) == 0)
		exit_status = pwd_cmd();
	else if (ft_strncmp(cmd->cmd_arr[0], "export", 7) == 0)
		exit_status = export_cmd(cmd, env);
	else if (ft_strncmp(cmd->cmd_arr[0], "unset", 6) == 0)
		unset_cmd(cmd, env);
	else if (ft_strncmp(cmd->cmd_arr[0], "env", 4) == 0)
		exit_status = env_cmd(cmd, *env);
	else if (ft_strncmp(cmd->cmd_arr[0], "exit", 5) == 0)
		exit_status = exit_cmd(cmd, *env);
	if (exit_status == 1)
		cmd->exit_status = 1;
}
