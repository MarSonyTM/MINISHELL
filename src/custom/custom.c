#include "../../inc/minishell.h"

void	custom_exec(t_cmd *cmd, t_env **env)
{
	if (ft_strncmp(cmd->cmd_arr[0], "echo", 5) == 0)
		echo_cmd(cmd);
	else if (ft_strncmp(cmd->cmd_arr[0], "cd", 3) == 0)
		cd_cmd(cmd);
	else if (ft_strncmp(cmd->cmd_arr[0], "pwd", 4) == 0)
		pwd_cmd();
	else if (ft_strncmp(cmd->cmd_arr[0], "export", 7) == 0)
		export_cmd(cmd, env);
	else if (ft_strncmp(cmd->cmd_arr[0], "unset", 6) == 0)
		unset_cmd(cmd, env);
	else if (ft_strncmp(cmd->cmd_arr[0], "env", 4) == 0)
		env_cmd(cmd, *env);
	else if (ft_strncmp(cmd->cmd_arr[0], "exit", 5) == 0)
		exit_cmd(cmd, *env);
	// exit(EXIT_SUCCESS);
}
