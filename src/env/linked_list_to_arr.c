#include "../../inc/minishell.h"
#include <unistd.h>

int	get_len(t_env *env)
{
	t_env	*tmp;	
	int		len;

	len = 0;
	tmp = env;
	while (tmp != NULL)
	{
		len++;
		tmp = tmp->next;
	}
	return (len);
}

char	*convert_one_node(t_env *env)
{
	char	*tmp_str;
	char	*entry;

	tmp_str = ft_strjoin(env->key, "=");
	if (!tmp_str)
		return (NULL);
	entry = ft_strjoin(tmp_str, env->value);
	free(tmp_str);
	return (entry);
}

char	**allocate_envp(t_cmd *cmd, t_env **env, int len)
{
	char	**envp;

	envp = (char **)malloc(sizeof(char *) * (len + 1));
	if (!envp)
	{
		clean_up(cmd, *env);
		return (NULL);
	}
	return (envp);
}

char	**env_to_array(t_cmd *cmd, t_env **env)
{
	t_env	*tmp;	
	char	**envp;
	int		i;
	int		len;

	i = 0;
	len = get_len(*env);
	tmp = *env;
	envp = allocate_envp(cmd, env, len);
	if (!envp)
		return (NULL);
	while (tmp != NULL)
	{
		envp[i] = convert_one_node(tmp);
		if (!envp[i])
		{
			clean_up(cmd, *env);
			free(envp);
			return (NULL);
		}
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}
