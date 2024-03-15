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

static int	handle_redirect(int fd, int mode, char *file)
{
	if (fd == -1)
	{
		ft_putendl_fd("error", 2);
		if (mode == 0)
			error(ERR_FIL, file);
		else
			error(ERR_PERM, file);
		return (1);
	}
	if (mode == 0)
	{
		if (dup2(fd, 0) == -1)
			return(1);
	}
	else 
	{
		if (dup2(fd, 1) == -1)
			return (1);
	}
	close(fd);
	return (0);
}

int	redirection(char *file, int mode, int custom)
{
	int	fd;

	ft_putendl_fd("redirection", 1);
	if (mode == 0)
		fd = open(file, O_RDONLY);
	else if (mode == 2)
		fd = open(file, O_WRONLY  | O_CREAT | O_APPEND, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);	
	if (handle_redirect(fd, mode, file) == 1)
	{
		if (!custom)
			exit(1);
		return (1);
	}
	return (0);
}
