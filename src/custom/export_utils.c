#include "../../inc/minishell.h"

char	*get_key(char *cmd, int *j)
{
	while (cmd[*j] && cmd[*j] != '=')
		(*j)++;
	return (ft_substr(cmd, 0, *j));
}

char	*get_value(char *cmd, int *j)
{
	*j = *j + 2;
	while (cmd[*j] && cmd[*j] != ':')
		(*j)++;
	return (ft_substr(cmd, *j + 1, ft_strlen(cmd) - *j - 1));
}
