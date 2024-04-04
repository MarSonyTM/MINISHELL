/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_command_path1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 12:12:11 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/04 12:30:16 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*construct_full_path_parser(char *dir, char *command,
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

char	*check_path_accessibility(char *fullPath, char *pathCopy)
{
	if (access(fullPath, F_OK) == 0 && access(fullPath, X_OK) == 0)
	{
		free(pathCopy);
		return (fullPath);
	}
	free(fullPath);
	return (NULL);
}

char	*resolve_command_path(char *command, t_env *env)
{
	t_command_path	cp;

	cp.path = ft_getenv("PATH", env);
	cp.path_copy = ft_strdup(cp.path);
	if (!cp.path_copy)
		return (NULL);
	cp.dir = ft_strtok(cp.path_copy, ":");
	cp.command_len = ft_strlen(command);
	while (cp.dir != NULL)
	{
		cp.dir_len = ft_strlen(cp.dir);
		cp.full_path = construct_full_path_parser(cp.dir, command,
				cp.dir_len, cp.command_len);
		if (cp.full_path == NULL)
			return (free(cp.path_copy), NULL);
		cp.accessible_path = check_path_accessibility(cp.full_path,
				cp.path_copy);
		if (cp.accessible_path != NULL)
			return (cp.accessible_path);
		cp.dir = ft_strtok(NULL, ":");
	}
	free(cp.path_copy);
	error(ERR_CMD, command);
	return (NULL);
}
