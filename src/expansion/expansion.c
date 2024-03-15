#include "../../inc/minishell.h"

void expand_env_vars(t_cmd *cmd, t_env *env)
{
    t_env *tmp = env;
    char *var_value = NULL;

    while (cmd != NULL)
    {
        int i = 0;
        while (cmd->env_vars[i] != NULL) 
        {
            // Remove the $ symbol from the environment variable name
            char *env_var_name = cmd->env_vars[i] + 1;

            printf("Expanding variable: %s\n", env_var_name); // Debug print

            while (tmp != NULL)
            {
                if (ft_strcmp(tmp->key, env_var_name) == 0)
                {
                    var_value = ft_strdup(tmp->value);
                    break;
                }
                tmp = tmp->next;
            }
            if (var_value != NULL)
            {
                printf("Variable value: %s\n", var_value); // Debug print

                // Replace the env_var field with the environment variable's value
                free(cmd->env_vars[i]);
                cmd->env_vars[i] = ft_strdup(var_value);

                // Store the value in cmd_arr for later use by the echo command
                free(cmd->cmd_arr[i + 1]);
                cmd->cmd_arr[i + 1] = ft_strdup(var_value);
                cmd->cmd_arr[i + 2] = NULL;
            } 
            else 
            {
                // Handle error: Environment variable not found
                printf("Error: Environment variable '%s' not found\n", env_var_name); // Debug print
            }
            tmp = env;
            i++;
        }
        cmd = cmd->next;
    }
}