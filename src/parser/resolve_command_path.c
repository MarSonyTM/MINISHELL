/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_command_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:53:44 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/03 12:50:24 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*resolve_command_path(char *command, t_env *env)
{
	char	*path;
	char	*path_copy;
	char	*dir;
	size_t	command_len;
	size_t	dir_len;
	char	*full_path;

	path = ft_getenv("PATH", env);
	if (!path)
		return (NULL);
	path_copy = ft_strdup(path);
	if (!path_copy)
		return (NULL);
	dir = ft_strtok(path_copy, ":");
	command_len = ft_strlen(command);
	while (dir != NULL)
	{
		dir_len = ft_strlen(dir);
		full_path = malloc(dir_len + command_len + 2);
		if (full_path == NULL)
			return (free(path_copy), NULL);
		ft_strcpy(full_path, dir);
		full_path[dir_len] = '/';
		ft_strcpy(full_path + dir_len + 1, command);
		if (access (full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
			return (free(path_copy), full_path);
		free(full_path);
		dir = ft_strtok(NULL, ":");
	}
	free(path_copy);
	error(ERR_CMD, command);
	return (NULL);
}

static char	**init_env_vars(void)
{
	char	**vars;

	vars = malloc(sizeof(char *) * 1);
	if (!vars)
		return (NULL);
	vars[0] = NULL;
	return (vars);
}

static char	**init_cmd_arr(void)
{
	char	**arr;

	arr = malloc(sizeof(char *) * 2);
	if (!arr)
		return (NULL);
	arr[0] = NULL;
	return (arr);
}


static int	initialize_cmd_struct(t_cmd *cmd)
{
	cmd->cmd_path = NULL;
	cmd->cmd_arr = init_cmd_arr();
	cmd->input = NULL;
	cmd->exit_status_token = NULL;
	cmd->env_vars = init_env_vars();
	cmd->output = NULL;
	cmd->exit_status = 0;
	cmd->next = NULL;
	if (!cmd->cmd_arr || !cmd->env_vars)
	{
		free(cmd->cmd_arr);
		free(cmd->env_vars);
		return (1);
	}
	return (0);
}


t_cmd	*new_command(t_cmd **cmd)
{
	t_cmd	*new_cmd;
	t_cmd	*last;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd || initialize_cmd_struct(new_cmd))
		return (free(new_cmd), NULL);
	if (*cmd == NULL)
		*cmd = new_cmd;
	else
	{
		last = *cmd;
		while (last->next != NULL)
			last = last->next;
		last->next = new_cmd;
	}
	return (new_cmd);
}


