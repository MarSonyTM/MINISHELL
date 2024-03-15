#include "../../inc/minishell.h"

char	*get_key(char *cmd, int *j)
{
	while (cmd[*j] && cmd[*j] != '=')
		(*j)++;
	return (ft_substr(cmd, 0, *j));
}

char	*get_value_concat(char *cmd, int *j)
{
	*j = *j + 2;
	while (cmd[*j] && cmd[*j] != ':')
		(*j)++;
	return (ft_substr(cmd, *j, ft_strlen(cmd) - *j));
}

char    *get_value(char *cmd, int *j)
{
    *j = *j + 1;
    return (ft_substr(cmd, *j, ft_strlen(cmd) - *j));
}

void    free_env(t_env *env)
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

t_env	*arr_to_linked_list(char **envp)
{
    t_env	*env;
    t_env   *tmp;
    int		i;
    int		j;
    char	*key;
    char	*value;

    env = malloc(sizeof(t_env));
    if (!env)
        exit (1);
    tmp = env;
    i = 0;
    while (envp[i] != NULL)
    {
        j = 0;
        key = get_key(envp[i], &j);
        value = get_value(envp[i], &j);
        tmp->key = key;
        tmp->value = value;
        if (envp[i + 1] != NULL)
        {
            tmp->next = malloc(sizeof(t_env));
            if (!tmp->next)
            {
                free_env(env);
                exit (1);
            }
            tmp = tmp->next;
        }
        else
            tmp->next = NULL;
        i++;
    }
    return (env);
}
