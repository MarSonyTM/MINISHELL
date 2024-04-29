/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csturm <csturm@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:50:42 by csturm            #+#    #+#             */
/*   Updated: 2024/04/29 08:53:58 by csturm           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	update_env_var(t_env *env_var, char *key, char *value)
{
	free(env_var->key);
	free(env_var->value);
	env_var->key = key;
	env_var->value = value;
}

t_env	*find_env_var(t_env *env, char *key)
{
	while (env != NULL)
	{
		if (!ft_strncmp(env->key, key, ft_strlen(key) + 1))
			return (env);
		env = env->next;
	}
	return (NULL);
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

int	handle_existing_var(t_env *tmp, char *key, char *value)
{
	tmp->value = ft_strjoin(tmp->value, value);
	free(key);
	free(value);
	if (!tmp->value)
		return (1);
	return (0);
}

int	handle_new_var(t_env **env, char *key, char *value)
{
	*env = add_env_node(env, key, value);
	if (!*env)
		return (1);
	return (0);
}
