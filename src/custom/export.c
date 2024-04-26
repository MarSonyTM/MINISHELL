/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csturm <csturm@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:50:43 by csturm            #+#    #+#             */
/*   Updated: 2024/04/24 17:20:19 by csturm           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_env	*add_env_node(t_env **env, char *key, char *value)
{
	t_env	*new;
	t_env	*tmp;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	if (!*env)
		return (new);
	tmp = *env;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
	return (*env);
}

int	add_empty_env_var(char *cmd, t_env **env)
{
	t_env	*tmp;
	char	*key;
	char	*value;

	key = ft_strdup(cmd);
	value = ft_strdup("");
	tmp = *env;
	while (tmp != NULL)
	{
		if (!ft_strncmp(tmp->key, key, ft_strlen(key) + 1))
		{
			free(tmp->value);
			tmp->value = value;
			break ;
		}
		tmp = tmp->next;
	}
	if (!tmp)
	{
		*env = add_env_node(env, key, value);
		if (!*env)
			return (1);
	}
	return (0);
}

int	add_new_env_var(char *cmd, t_env **env, int j)
{
	t_env	*tmp;
	char	*key;
	char	*value;

	while (cmd[j] && cmd[j] != '=')
		j++;
	key = ft_substr(cmd, 0, j);
	if (!key)
		return (1);
	value = ft_strdup(cmd + j + 1);
	if (!value)
		return (1);
	tmp = find_env_var(*env, key);
	if (tmp)
		update_env_var(tmp, key, value);
	else
	{
		*env = add_env_node(env, key, value);
		if (!*env)
			return (1);
	}
	return (0);
}

int	no_colon(char *cmd, t_env **env, int j)
{
	char	*key;

	key = get_key(cmd, &j);
	if (!key)
		return (1);
	if (add_empty_env_var(key, env) == 1)
		return (1);
	return (0);
}

int	concatenate_env_var(char *cmd, t_env **env, int j)
{
	t_env	*tmp;
	char	*key;
	char	*value;

	if (ft_strchr(cmd, ':') == NULL)
		return (no_colon(cmd, env, j) == 1);
	key = get_key(cmd, &j);
	if (!key)
		return (1);
	value = get_value_concat(cmd, &j);
	if (!value)
		return (free(key), 1);
	tmp = find_env_var(*env, key);
	if (tmp)
	{
		tmp->value = ft_strjoin(tmp->value, value);
		free(key);
		free(value);
	}
	else
	{
		*env = add_env_node(env, key, value);
		if (!*env)
			return (1);
	}
	return (0);
}

int	export_cmd(t_cmd *cmd, t_env **env)
{
	int	i;

	i = 1;
	if (!cmd->cmd_arr[1])
		return (env_cmd(cmd, *env), 0);
	if (cmd->cmd_arr[1][0] == '=' || ft_isdigit(cmd->cmd_arr[1][0]))
	{
		error(ERR_ARG, "export", cmd->cmd_arr[1], 1);
		return (1);
	}
	while (cmd->cmd_arr[i])
	{
		if (ft_strchr(cmd->cmd_arr[i], '='))
		{
			if (ft_strchr(cmd->cmd_arr[i], '$')
				&& concatenate_env_var(cmd->cmd_arr[i], env, 0))
				return (-1);
			else if (add_new_env_var(cmd->cmd_arr[i], env, 0) == 1)
				return (-1);
		}
		else if (add_empty_env_var(cmd->cmd_arr[i], env) == 1)
			return (-1);
		i++;
	}
	return (0);
}
