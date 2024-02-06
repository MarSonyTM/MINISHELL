/* This is how I split all the paths in the env variable
into strings that I could add the commands to. 

Keep in mind that I am directly working with the envp
char array, which apparently we should not do.*/

char	*append_slash(char *path)
{
	char	*new_path;

	new_path = ft_strjoin(path, "/");
	if (!new_path)
		return (NULL);
	free(path);
	return (new_path);
}

char	**find_paths(char **envp)
{
	char	**paths;
	char	*new_path;
	int		i;
	int		j;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	paths = ft_split(envp[i], ':');
	if (!paths)
		return (NULL);
	j = 0;
	while (paths[j] != NULL)
	{
		paths[j] = append_slash(paths[j]);
		if (!paths[j])
			return (NULL);
		j++;
	}
	new_path = ft_strtrim(paths[0], "PATH=");
	if (!new_path)
		return (NULL);
	free(paths[0]);
	paths[0] = new_path;
	return (paths);
}

/* I then take my command and concatenate it to one string
after another until I find the one that is fully 
accessible. That one I save as cmd_path. 

It is possible to input the whole command path instead of
the command. That is why I am checking for that separately.*/

int	check_cmd(char *cmd)
{
	if (access(cmd, F_OK) != -1)
	{
		if (access(cmd, X_OK) != -1)
			return (0);
	}
	return (1);
}

char	*check_cmd_path(char **paths, char *cmd, int i)
{
	char	*cmd_path;

	cmd_path = ft_strjoin(paths[i], cmd);
	if (!cmd_path)
		return (NULL);
	if (access(cmd_path, F_OK) != -1)
	{
		if (access(cmd_path, X_OK) != -1)
		{
			free_array(paths);
			return (cmd_path);
		}
	}
	free(cmd_path);
	return (NULL);
}

char	*find_cmd_path(char *cmd, char **envp)
{
	char	*cmd_path;
	char	**paths;
	int		i;

	if (!check_cmd(cmd))
		return (cmd);
	paths = find_paths(envp);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		cmd_path = check_cmd_path(paths, cmd, i);
		if (cmd_path)
			return (cmd_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}