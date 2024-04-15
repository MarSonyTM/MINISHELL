/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   determine_token_type1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csturm <csturm@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:14:46 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/15 19:05:38 by csturm           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	is_command(char *token, t_lexer *lexer)
{
	char	*path;
	char	*path_copy;
	char	*dir;
	char	*full_path;

	path = NULL;
	path_copy = ft_strdup(path);
	path = ft_getenv("PATH", lexer->env);
	if (!path)
		return (false);
	if (!path_copy)
		return (1);
	dir = ft_strtok(path_copy, ":");
	while (dir)
	{
		full_path = construct_full_path(dir, token);
		free(full_path);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			free(path_copy);
			return (true);
		}
		dir = ft_strtok(NULL, ":");
	}
	free(path_copy);
	return (false);
}

char	*construct_full_path(char *dir, char *token)
{
	size_t	full_path_len;
	char	*full_path;

	full_path_len = ft_strlen(dir) + ft_strlen(token) + 2;
	full_path = malloc(full_path_len);
	if (!full_path)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	ft_strcpy(full_path, dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, token);
	return (full_path);
}
