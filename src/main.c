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
    t_cmd *cmd;
    t_cmd *tmp;

    env = NULL;
    cmd = malloc(sizeof(t_cmd));
    if (!cmd)
    {
        /* error */
    }
    tmp = cmd;
    tmp->cmd_arr = malloc(sizeof(char *) * 4);
    if (!cmd->cmd_arr)
    {
        /* error */
    }
    tmp->cmd_arr[0] = ft_strdup("echo");
    tmp->cmd_arr[1] = ft_strdup("hello");
    tmp->cmd_arr[2] = ft_strdup("there");
    tmp->cmd_arr[3] = NULL;
    tmp->cmd_path = NULL;
    tmp->input = NULL;
    tmp->output = NULL;
    tmp->exit_status = 0;

    tmp->next = malloc(sizeof(t_cmd));
    if (!tmp->next)
    {
        /* error */
    }
    tmp = tmp->next;
    tmp->cmd_arr = malloc(sizeof(char *) * 2);
    if (!tmp->cmd_arr)
    {
        /* error */
    }
    tmp->cmd_arr[0] = ft_strdup("wc");
    tmp->cmd_arr[1] = NULL;
    tmp->cmd_path = ft_strdup("/usr/bin/wc");
    tmp->input = NULL;
    tmp->output = NULL;
    tmp->exit_status = 0;
    tmp->next = NULL;
    executor(cmd, env);
    clean_up(cmd, env);
    return (0);
}
 