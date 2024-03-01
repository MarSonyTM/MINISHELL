#include "../../inc/minishell.h"

static void    duplicate_fd(int old_fd, int new_fd)
{
    if(dup2(old_fd, new_fd) == -1)
    {
        ft_putstr_fd("dup2 error\n", 0);
        /* error */
    }
    close (old_fd);
}

static void    child_process(t_cmd *cmd, int i, t_exec *exec, t_env *env)
{
    ft_putstr_fd("child_process\n", 1);
    char **envp;

    if (i != exec->processes - 1)
        duplicate_fd(exec->fd[1], 1);
    ft_putchar_fd('1', 0);
    ft_putchar_fd('1', 1);
    if (i != 0)
        duplicate_fd(exec->old_fd[0], 0);
    ft_putchar_fd('2', 0);
    ft_putchar_fd('2', 1);
    if (i != exec->processes -1)
        close(exec->fd[0]);
    ft_putchar_fd('3', 0);
    ft_putchar_fd('3', 1);
    if (cmd->input)
        redirection(cmd->input, 0);
    ft_putchar_fd('4', 0);
    ft_putchar_fd('4', 1);
    if (cmd->output)
        redirection(cmd->output, 1);
    ft_putchar_fd('5', 0);
    ft_putchar_fd('5', 1);
    if (cmd->cmd_path == NULL)
        custom_exec(cmd, env);
    else
    {
        ft_putstr_fd("execve\n", 0);
        envp = env_to_array(cmd, env);
        if (execve(cmd->cmd_path, cmd->cmd_arr, envp) == -1)
        {
            /* error */
            exit (1);
        }
    }
}

static void    create_child_process(t_cmd *cmd, int i, t_exec *exec, t_env *env)
{
    if (pipe(exec->fd) == -1)
        exit(1);
    exec->pid[i] = fork();
    if (exec->pid[i] == -1)
        exit(1);
    if (exec->pid[i] == 0)
    {
        child_process(cmd, i, exec, env);
        exit(0);
    }
    close (exec->fd[1]);
    if (i != 0)
    {
        close(exec->old_fd[0]);
    }
    exec->old_fd[0] = exec->fd[0];
    exec->old_fd[1] = exec->fd[1];
    close (exec->fd[1]);
}

/* how to keep track of open fds? --> struct (linked list)? */

int    executor(t_cmd *cmd, t_env *env)
{
    t_exec  exec;
    int     i;
    int     j;

    exec.processes = count_processes(cmd);
    exec.old_fd[0] = -1;
    exec.old_fd[1] = -1;
    exec.pid = (int *)malloc(sizeof(int) * exec.processes);
    if (!exec.pid)
        clean_up(cmd, env);
    exec.status = (int *)malloc(sizeof(int) * exec.processes);
    if (!exec.status)
    {
        free(exec.pid);
        clean_up(cmd, env);
    }
    if (ft_strncmp(cmd->cmd_arr[0], "cd", 3) == 0)
    {
        cd_cmd(cmd);
        return (0);
    }
    i = 0;    
    while (exec.processes > 0)
    {
        ft_putnbr_fd(i, 1);
        create_child_process(cmd, i, &exec, env);
        exec.processes--;
        i++;
        if (cmd->next != NULL)
            cmd = cmd->next;
    }
    j = 0;
    while (j < exec.processes)
    {
        waitpid(exec.pid[j], &exec.status[j], 0);
        if (WIFEXITED(exec.status[j]) && WEXITSTATUS(exec.status[j]) != 0)
            exit(1);
    }
    free(exec.pid);
    free(exec.status);
    close(exec.old_fd[0]);
    close(exec.old_fd[1]);
    return (0);
}
