/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csturm <csturm@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:50:29 by csturm            #+#    #+#             */
/*   Updated: 2024/04/16 15:58:15 by csturm           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_custom(t_cmd *cmd, t_env **env, t_exec **exec)
{
	if (ft_strncmp(cmd->cmd_arr[0], "echo", 5) == 0)
		echo_cmd(cmd);
	else if (ft_strncmp(cmd->cmd_arr[0], "cd", 3) == 0)
		return (cd_cmd(cmd, *env));
	else if (ft_strncmp(cmd->cmd_arr[0], "pwd", 4) == 0)
		return (pwd_cmd());
	else if (ft_strncmp(cmd->cmd_arr[0], "export", 7) == 0)
		return (export_cmd(cmd, env));
	else if (ft_strncmp(cmd->cmd_arr[0], "unset", 6) == 0)
		unset_cmd(cmd, env);
	else if (ft_strncmp(cmd->cmd_arr[0], "env", 4) == 0)
		return (env_cmd(cmd, *env));
	else if (ft_strncmp(cmd->cmd_arr[0], "exit", 5) == 0)
		return (exit_cmd(cmd, *env, exec));
	return (0);
}

void	custom_exec(t_cmd *cmd, t_env **env, t_exec **exec)
{
	cmd->exit_status = execute_custom(cmd, env, exec);
}
