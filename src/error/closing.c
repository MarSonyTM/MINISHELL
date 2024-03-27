#include "../../inc/minishell.h"

void	close_and_free(t_exec *exec)
{
	free(exec->pid);
	free(exec->status);
	if (exec->processes > 1)
	{
		close(exec->old_fd[0]);
		exec->open_fds[(exec->processes - 1) * 2 - 2] = -1;
		close(exec->old_fd[1]);
		exec->open_fds[(exec->processes - 1) * 2 - 1] = -1;
	}
	close_fds(exec->open_fds, exec->processes);	
}

void	close_fds(int *open_fds, int processes)
{
	int	i;

	i = 0;
	while (i < processes * 2)
	{
		if (open_fds[i] != -1)
			close(open_fds[i]);
		i++;
	}
	free(open_fds);
}



/* figure out how to close all open fds */

 
