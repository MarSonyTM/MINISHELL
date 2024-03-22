#include "../../inc/minishell.h"

static t_env	*add_env_node(t_env **env, char *key, char *value)
{
	t_env	*new;
	t_env	*tmp;

	new = malloc(sizeof(t_env));
	new->key = key;
	new->value = value;
	new->next = NULL;
	if (env == NULL)
		return (new);
	tmp = *env;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
	return (*env);
}

static void	add_empty_env_var(char *cmd, t_env **env)
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
		*env = add_env_node(env, key, value);
}

static void	add_new_env_var(char *cmd, t_env **env, int j)
{
	t_env	*tmp;
	char	*key;
	char	*value;

	while (cmd[j] && cmd[j] != '=')
		j++;
	key = ft_substr(cmd, 0, j);
	value = ft_strdup(cmd + j + 1);
	tmp = *env;
	while (tmp != NULL)
	{
		if (!ft_strncmp(tmp->key, key, ft_strlen(key)))
		{
			free(tmp->key);
			free(tmp->value);
			tmp->key = key;
			tmp->value = value;
			break ;
		}
		tmp = tmp->next;
	}
	if (!tmp)
		*env = add_env_node(env, key, value);
}

static void	concatenate_env_var(char *cmd, t_env **env, int j)
{
	t_env	*tmp;
	char	*key;
	char	*value;

	if (ft_strchr(cmd, ':') == NULL)
	{
		key = get_key(cmd, &j);
		add_empty_env_var(key, env);
		return ;
	}
	key = get_key(cmd, &j);
	value = get_value_concat(cmd, &j);
	tmp = *env;
	while (tmp != NULL && ft_strncmp(tmp->key, key, ft_strlen(key)))
		tmp = tmp->next;
	if (tmp)
	{
		tmp->value = ft_strjoin(tmp->value, value);
		free(key);
		free(value);
	}
	else
		*env = add_env_node(env, key, value);
}

int	export_cmd(t_cmd *cmd, t_env **env)
{
	int	i;

	i = 1;
	if (!cmd->cmd_arr[1]) //export command with no argument
	{
		env_cmd(cmd, *env);
		return (1);
	}
	if (cmd->cmd_arr[1][0] == '=' || ft_isdigit(cmd->cmd_arr[1][0]))
	{
		error(ERR_VAL, "export", cmd->cmd_arr[1], 1);
		return (1);
	}
	while (cmd->cmd_arr[i])
	{
		if (ft_strchr(cmd->cmd_arr[i], '='))
		{
			if (ft_strchr(cmd->cmd_arr[i], '$'))
				concatenate_env_var(cmd->cmd_arr[i], env, 0);
			else
				add_new_env_var(cmd->cmd_arr[i], env, 0);
		}
		else
			add_empty_env_var(cmd->cmd_arr[i], env);
		i++;
	}
	return (0);
}
