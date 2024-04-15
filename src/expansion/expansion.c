/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianfurnica <marianfurnica@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 19:50:27 by marianfurni       #+#    #+#             */
/*   Updated: 2024/04/15 20:38:20 by marianfurni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_env_value(char *var_name, t_env *env)
{
	t_env	*current_env;

	current_env = env;
	while (current_env != NULL)
	{
		if (ft_strcmp(current_env->key, var_name) == 0)
		{
			return (ft_strdup(current_env->value));
		}
		current_env = current_env->next;
	}
	return (NULL);
}

int	get_cmd_arr_size(char ***cmd_arr)
{
	int	size;

	size = 0;
	while ((*cmd_arr)[size] != NULL)
	{
		size++;
	}
	return (size);
}

char	**create_new_cmd_arr(char ***cmd_arr, int size, char *value)
{
	int		i;
	char	**new_cmd_arr;

	i = 0;
	new_cmd_arr = malloc((size + 2) * sizeof(char *));
	if (new_cmd_arr == NULL)
		return (NULL);
	while (i < size)
	{
		new_cmd_arr[i] = (*cmd_arr)[i];
		i++;
	}
	new_cmd_arr[size] = ft_strdup(value);
	if (new_cmd_arr[size] == NULL)
	{
		free(new_cmd_arr);
		return (NULL);
	}
	new_cmd_arr[size + 1] = NULL;
	return (new_cmd_arr);
}

void	append_to_cmd_arr(char ***cmd_arr, char *value)
{
	int		size;
	char	**new_cmd_arr;

	size = get_cmd_arr_size(cmd_arr);
	new_cmd_arr = create_new_cmd_arr(cmd_arr, size, value);
	if (new_cmd_arr == NULL)
		return ;
	free(*cmd_arr);
	*cmd_arr = new_cmd_arr;
}

void	expand_env_vars(t_cmd *cmd, t_env *env)
{
	int		i;
	char	*var_start;

	while (cmd != NULL)
	{
		i = 0;
		while (cmd->env_vars[i] != NULL)
		{
			var_start = cmd->env_vars[i] + 1;
			process_env_var(var_start, cmd, env);
			i++;
		}
		cmd = cmd->next;
	}
}
