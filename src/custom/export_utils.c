/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csturm <csturm@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:50:42 by csturm            #+#    #+#             */
/*   Updated: 2024/04/19 16:51:16 by csturm           ###   ########.fr       */
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
