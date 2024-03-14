#include "../../inc/minishell.h"

void expand_env_vars(t_cmd *cmd)
{
    while (cmd != NULL)
    {
        if (cmd->env_var != NULL) 
        {
            // Remove the $ symbol from the environment variable name
            char *env_var_name = cmd->env_var + 1;

            printf("Expanding variable: %s\n", env_var_name); // Debug print

            char *var_value = getenv(env_var_name);
            if (var_value != NULL)
            {
                printf("Variable value: %s\n", var_value); // Debug print

                // Replace the env_var field with the environment variable's value
                free(cmd->env_var);
                cmd->env_var = ft_strdup(var_value);
            } 
            else 
            {
                // Handle error: Environment variable not found
                printf("Error: Environment variable '%s' not found\n", env_var_name); // Debug print
            }
        }
        cmd = cmd->next;
    }
}