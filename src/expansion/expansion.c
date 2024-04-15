#include "../../inc/minishell.h"

char *get_env_value(char *var_name, t_env *env)
{
    t_env *current_env = env;
    while (current_env != NULL)
    {
        if (ft_strcmp(current_env->key, var_name) == 0)
        {
            return ft_strdup(current_env->value);
        }
        current_env = current_env->next;
    }
    return NULL;
}

void append_to_cmd_arr(char ***cmd_arr, char *value)
{
    // Determine the current size of cmd_arr
    int size = 0;
    while ((*cmd_arr)[size] != NULL)
    {
        size++;
    }

    // Allocate a new block of memory for the expanded cmd_arr
    char **new_cmd_arr = malloc((size + 2) * sizeof(char *));
    if (new_cmd_arr == NULL)
    {
        // Handle allocation failure
        return;
    }

    // Copy the existing elements from cmd_arr to new_cmd_arr
    for (int i = 0; i < size; i++)
    {
        new_cmd_arr[i] = (*cmd_arr)[i];
    }

    // Add the new value to new_cmd_arr
    new_cmd_arr[size] = ft_strdup(value); // Allocate a new block for the value
    if (new_cmd_arr[size] == NULL)
    {
        // Handle allocation failure
        free(new_cmd_arr);
        return;
    }

    // Terminate the new array
    new_cmd_arr[size + 1] = NULL;

    // Free the old cmd_arr and update the pointer
    free(*cmd_arr);
    *cmd_arr = new_cmd_arr;
}

void expand_env_vars(t_cmd *cmd, t_env *env)
{
    while (cmd != NULL)
    {
        int i = 0; // Index for env_vars
        while (cmd->env_vars[i] != NULL)
        {
            char *var_start = cmd->env_vars[i] + 1; // Skip the initial $
            char *var_end = ft_strchr(var_start, '$'); // Find the next $

            while (var_end != NULL)
            {
                // Get the variable name
                char *var_name = strndup(var_start, var_end - var_start);

                // Expand the variable
                char *var_value = get_env_value(var_name, env);
                if (var_value != NULL)
                {
                    printf("Variable value: %s\n", var_value); // Debug print
                    append_to_cmd_arr(&cmd->cmd_arr, var_value);
                    free(var_value); // Free the memory allocated by strdup
                }
                free(var_name);
                // Move to the next variable
                var_start = var_end + 1;
                var_end = ft_strchr(var_start, '$');
            }

            // Expand the last variable (or the only variable if there were no additional $)
            char *var_value = get_env_value(var_start, env);
            if (var_value != NULL)
            {
                printf("Variable value: %s\n", var_value); // Debug print
                append_to_cmd_arr(&cmd->cmd_arr, var_value);
                free(var_value); // Free the memory allocated by strdup
            }
            i++;
        }
        cmd = cmd->next;
    }
}