/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_functions2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csturm <csturm@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:50:20 by csturm            #+#    #+#             */
/*   Updated: 2024/04/24 17:50:01 by csturm           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_for_digit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			error(ERR_NUM, "exit", str, 1);
			return (0);
		}
		i++;
	}
	return (1);
}

int	exit_cmd(t_cmd *cmd, t_env *env, t_exec **exec, int stdout_fd)
{
	int	exit_code;

	exit_code = EXIT_SUCCESS;
	if (cmd->cmd_arr[1])
	{
		if (check_for_digit(cmd->cmd_arr[1]))
		{
			/* check for int overflow */
			exit_code = ft_atoi(cmd->cmd_arr[1]);
			while (exit_code < 0)
				exit_code = 256 + exit_code;
			if (exit_code > 255)
				exit_code = exit_code % 256;
		}
		else
			exit_code = 2;
	}
	if (cmd->cmd_arr[1] && cmd->cmd_arr[2])
		return (error(ERR_ARGS, "exit", NULL, 1), 1);
	clean_up(cmd, env);
	close_and_free(*exec);
	ft_putendl_fd("exit", 1);
	if (stdout_fd != -1)
		close(stdout_fd);
	exit(exit_code);
	return (0);
}

static void	free_env_node(t_env **env, t_env *tmp, t_env *prev)
{
	if (prev == NULL)
		*env = tmp->next;
	else
		prev->next = tmp->next;
	free(tmp->key);
	free(tmp->value);
	free(tmp);
}

void	unset_cmd(t_cmd *cmd, t_env **env)
{
	t_env	*tmp;
	t_env	*prev;
	int		i;

	i = 1;
	while (cmd->cmd_arr[i] != NULL)
	{
		tmp = *env;
		prev = NULL;
		while (tmp != NULL)
		{
			if (ft_strncmp(tmp->key, cmd->cmd_arr[i],
					ft_strlen(cmd->cmd_arr[i]) + 1) == 0)
			{
				free_env_node(env, tmp, prev);
				break ;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
}
