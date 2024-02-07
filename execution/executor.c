#include "../inc/minishell.h"

void    child_process(t_cmd *cmd, int i, int *fd)
{
    if (i != cmd->processes -1)
        dup2(fd[1], 1);
    if (i != 0)
        dup2(fd[0], 0);
    close(fd[0]);
    close(fd[1]);
    if (cmd->input != NULL)
        open_input_file(cmd->input);
    if (cmd->output != NULL)
        open_output_file(cmd->output);
    if (execve(cmd->cmd_path, cmd->cmd_arr, cmd->env) == -1)
    {
        clean_up(cmd);
        exit(1);
    }
}

int    executor(t_cmd *cmd)
{
    int    fd[2];
    int    *pid;
    int    *status;
    int     i;

    i = 0;
    while (0 < cmd->processes)
    {
        if (pipe(fd) == -1)
            return (1);
        pid = (int *)malloc(sizeof(int) * cmd->processes);
        status = (int *)malloc(sizeof(int) * cmd->processes);
        if (!pid || !status)
            return (1);
        pid[i] = fork();
        if (pid[i] == -1)
            return (1);
        if (pid[i] == 0)
            child_process(cmd, i, fd);
        close(fd[1]);
        close(fd[0]);
        waitpid(pid[i], &status[i], 0);
        if (WIFEXITED(status[i]) && WEXITSTATUS(status[i]) != 0)
            return (1);
        cmd->processes--;
        i++;
        if (cmd->next != NULL)
            cmd = cmd->next;
    }
    return (0);
}
