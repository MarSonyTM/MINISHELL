#include "../../inc/minishell.h"

void	error(char *msg, char *ft)
{
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(ft, 2);
	ft_putstr_fd("\n", 2);
}