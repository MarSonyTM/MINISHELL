#include "../../inc/minishell.h"
 
 void expand_env_vars(t_cmd *cmd, t_env *env)
{
    while (cmd != NULL)
    {
        int i = 0; // Index for env_vars
        while (cmd->env_vars[i] != NULL)
        {
            // Remove the $ symbol from the environment variable name
            char *env_var_name = cmd->env_vars[i] + 1;
            printf("Expanding variable: %s\n", env_var_name); // Debug print

            // Find the value of the environment variable
            char *var_value = NULL;
            for (t_env *tmp = env; tmp != NULL; tmp = tmp->next)
            {
                if (ft_strcmp(tmp->key, env_var_name) == 0)
                {
                    var_value = ft_strdup(tmp->value);
                    break;
                }
            }

            if (var_value != NULL)
            {
                printf("Variable value: %s\n", var_value); // Debug print

                // Replace the env_var field with the environment variable's value
                free(cmd->env_vars[i]);
                cmd->env_vars[i] = ft_strdup(var_value);

                // Find the length of cmd_arr
                int cmd_arr_len = 0;
                while (cmd->cmd_arr[cmd_arr_len] != NULL) 
                {
                    cmd_arr_len++;
                }

                // Resize cmd_arr to accommodate the new value
                cmd->cmd_arr = realloc(cmd->cmd_arr, sizeof(char *) * (cmd_arr_len + 2));
                if (cmd->cmd_arr == NULL)
                {
                    // Handle realloc failure
                    printf("Error: Memory allocation failed\n");
                    return;
                }

                // Append the new value to cmd_arr
                cmd->cmd_arr[cmd_arr_len] = ft_strdup(var_value);
                // NULL-terminate cmd_arr
                cmd->cmd_arr[cmd_arr_len + 1] = NULL;

                free(var_value); // Remember to free the duplicated value after use
            }
            else
            {
                // Handle error: Environment variable not found
                printf("Error: Environment variable '%s' not found\n", env_var_name); // Debug print
            }
            i++;
        }
        cmd = cmd->next;
    }
}
