#include "../../inc/minishell.h"

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

char	**env_to_array(t_cmd *cmd, t_env **env)
{
	t_env	*tmp;	
	char	**envp;
	char	*tmp_str;
	int		i;
	int		len;

	i = 0;
	len = get_len(*env);
	tmp = *env;
	envp = (char **)malloc(sizeof(char *) * (len + 1));
	if (!envp)
		clean_up(cmd, *env);
	while (i < len)
		envp[i++] = ft_strdup("");
	i = 0;
	while (tmp != NULL)
	{
		tmp_str = ft_strjoin(tmp->key, "=");
		envp[i] = ft_strjoin(envp[i], tmp->value);
		free(tmp_str);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

static void	handle_redirect(int fd, int mode)
{
	if (fd == -1)
	{
		/* error ERR_FIL || ERR_PERM */
	}
	if (dup2(fd, mode) == -1)
	{
		/* error ERROR*/
	}
	close(fd);
}

void	redirection(char *file, int mode)
{
	int	fd;

	if (mode == 0)
		fd = open(file, O_RDONLY);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	handle_redirect(fd, mode);
}
