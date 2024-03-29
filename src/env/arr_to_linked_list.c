#include "../../inc/minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env != NULL)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

t_env	*create_env_node(char *envp, int *j)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = get_key(envp, j);
	new->value = get_value(envp, j);
	new->next = NULL;
	return (new);
}

t_env	*arr_to_linked_list(char **envp)
{
	t_env	*env;
	t_env	*tmp;
	int		i;
	int		j;

	env = create_env_node(envp[0], &j);
	if (!env)
		exit (1);
	tmp = env;
	i = 1;
	while (envp[i] != NULL)
	{
		j = 0;
		tmp->next = create_env_node(envp[i], &j);
		if (!tmp->next)
		{
			free_env(env);
			exit (1);
		}
		tmp = tmp->next;
		i++;
	}
	return (env);
}
