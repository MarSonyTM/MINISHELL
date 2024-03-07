 #include "../inc/minishell.h"

/* int main(void)
{
    char *input;
    while (1)
    {
        input = readline(PROMPT);
        if (!input)
            break ;
        if (*input)
            add_history(input);
        printf("You entered: %s\n", input);
        free(input);
    }
    return (0);
} */

int main (void)
{
    t_env *env;
    t_env *tmp_env;
    t_cmd *cmd;
    t_cmd *tmp;

    cmd = malloc(sizeof(t_cmd));
    if (!cmd)
    {
        /* error */
    }
    tmp = cmd;
    tmp->cmd_arr = malloc(sizeof(char *) * 3);
    if (!cmd->cmd_arr)
    {
        /* error */
    }
    tmp->cmd_arr[0] = ft_strdup("export");
    tmp->cmd_arr[1] = ft_strdup("NEW=value");
    tmp->cmd_arr[2] = NULL;
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
    // tmp->cmd_arr = malloc(sizeof(char *) * 2);
    // if (!tmp->cmd_arr)
    // {
    //     /* error */
    // }
    // tmp->cmd_arr[0] = ft_strdup("env");
    // tmp->cmd_arr[1] = NULL;
    // tmp->cmd_path = NULL;
    // tmp->input = NULL;
    // tmp->output = ft_strdup("output.txt");
    // tmp->exit_status = 0;
    tmp->next = NULL;

    env = malloc(sizeof(t_env));
    if (!env)
    {
        /* error */
    }
    tmp_env = env;
    tmp_env->key = ft_strdup("PATH");
    tmp_env->value = ft_strdup("/usr/bin:/bin");

    tmp_env->next = malloc(sizeof(t_env));
    if (!tmp_env->next)
    {
        /* error */
    }
    tmp_env = tmp_env->next;
    tmp_env->key = ft_strdup("HOME");
    tmp_env->value = ft_strdup("/home/user");
    tmp_env->next = NULL;

    executor(cmd, env);
    env_cmd(cmd, env);
    clean_up(cmd, env);
    return (0);
}
 