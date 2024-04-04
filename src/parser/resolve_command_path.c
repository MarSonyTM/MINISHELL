/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_command_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:53:44 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/04 14:00:00 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*create_full_path(char *dir, char *command,
		size_t dirLen, size_t commandLen)
{
	char	*full_path;

	full_path = malloc(dirLen + commandLen + 2);
	if (full_path == NULL)
	{
		return (NULL);
	}
	ft_strcpy(full_path, dir);
	full_path[dirLen] = '/';
	ft_strcpy(full_path + dirLen + 1, command);
	return (full_path);
}

bool	is_command_found(char *fullPath)
{
	return (access(fullPath, F_OK) == 0 && access(fullPath, X_OK) == 0);
}

char	*find_command_in_path(char *command, char *pathCopy, size_t commandLen)
{
	char	*dir;
	size_t	dir_len;
	char	*full_path;

	dir = ft_strtok(pathCopy, ":");
	while (dir != NULL)
	{
		dir_len = ft_strlen(dir);
		full_path = create_full_path(dir, command, dir_len, commandLen);
		if (full_path == NULL)
		{
			return (NULL);
		}
		if (is_command_found(full_path))
		{
			return (full_path);
		}
		free(full_path);
		dir = ft_strtok(NULL, ":");
	}
	return (NULL);
}

char	*resolve_command_path(char *command, t_env *env)
{
	char	*path;
	char	*path_copy;
	size_t	command_len;
	char	*full_path;

	path = ft_getenv("PATH", env);
	path_copy = ft_strdup(path);
	if (!path_copy)
	{
		return (NULL);
	}
	command_len = ft_strlen(command);
	full_path = find_command_in_path(command, path_copy, command_len);
	free(path_copy);
	if (full_path != NULL)
	{
		return (full_path);
	}
	error(ERR_CMD, command);
	return (NULL);
}
