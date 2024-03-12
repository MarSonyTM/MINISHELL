#include "../inc/minishell.h"
#include <stdio.h>

/* int main(void)
{
    if (argc != 1 || argv[1])
        exit(printf("This program takes no arguments\n"));

    while (1)
    {
        char *input = readline(PROMPT);
        if (!input)
            break ;
        if (*input)
            add_history(input);
        printf("You entered: %s\n", input);
        free(input);
    }
    return (0);
} */

int main (int argc, char **argv, char **envp)
{
    t_env *env;
    // t_env *tmp_env;
    t_cmd *cmd;
    t_cmd *tmp;
    int    how_many_cmds;
    char  *input;

    how_many_cmds = argc;
    input = argv[0];

    cmd = malloc(sizeof(t_cmd));
    if (!cmd)
    {
        /* error */
    }
    tmp = cmd;
    tmp->cmd_arr = malloc(sizeof(char *) * 2);
    if (!cmd->cmd_arr)
    {
        /* error */
    }
    tmp->cmd_arr[0] = ft_strdup("env");
    tmp->cmd_arr[1] = NULL;
    tmp->cmd_path = NULL;
    tmp->input = NULL;
    tmp->output = NULL;
    tmp->exit_status = 0;

    // tmp->next = malloc(sizeof(t_cmd));
    // if (!tmp->next)
    // {
    //     /* error */
    // }
    // tmp = tmp->next;

    // tmp->next = NULL;

    // env = malloc(sizeof(t_env));
    // if (!env)
    // {
    //     /* error */
    // }
    // tmp_env = env;
    // tmp_env->key = ft_strdup("PATH");
    // tmp_env->value = ft_strdup("/usr/bin:/bin");

    // tmp_env->next = malloc(sizeof(t_env));
    // if (!tmp_env->next)
    // {
    //     /* error */
    // }
    // tmp_env = tmp_env->next;
    // tmp_env->key = ft_strdup("HOME");
    // tmp_env->value = ft_strdup("/home/user");
    // tmp_env->next = NULL;

    env = arr_to_linked_list(envp);

    executor(cmd, &env);
    clean_up(cmd, env);          

    return (0);
}
