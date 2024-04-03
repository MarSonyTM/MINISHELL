#include "../../inc/minishell.h"

// Function to resolve the path of a command
t_cmd *initialize_new_command(t_cmd **cmd, t_token *current_token, t_env *env)
{
    t_cmd *new_cmd = new_command(cmd); // Create and link a new command
    if (!new_cmd) return NULL; // Handle failure

    // Allocate space for command name and NULL terminator
    new_cmd->cmd_arr = malloc(sizeof(char *) * 2);
    if (!new_cmd->cmd_arr) return NULL; // Handle failure

    new_cmd->cmd_arr[0] = ft_strdup(current_token->value); // Copy command/builtin name
    if (!new_cmd->cmd_arr[0]) return NULL; // Handle failure
    new_cmd->cmd_arr[1] = NULL; // NULL terminate the command array

    // For TOKEN_COMMAND, resolve the path
    if (current_token->type == TOKEN_COMMAND) {
        new_cmd->cmd_path = resolve_command_path(current_token->value, env);
        if (!new_cmd->cmd_path) return NULL; // Handle failure
    } else {
        new_cmd->cmd_path = NULL; // Builtin commands don't have a path
    }

    return new_cmd;
}

// Adds an argument to the current command
int add_argument_to_command(t_cmd *current_cmd, const char *arg_value)
{
    if (current_cmd == NULL || arg_value == NULL) 
        return (1); // Error condition
    
    // Increment arg_count for this command
    int arg_count = 0;
    while (current_cmd->cmd_arr[arg_count] != NULL) {
        arg_count++;
    }

    // Resize the cmd_arr for the new argument
    char **temp = realloc(current_cmd->cmd_arr, sizeof(char*) * (arg_count + 2)); // +1 for new arg, +1 for NULL
    if (!temp) 
        return (1); // Memory allocation error
    
    current_cmd->cmd_arr = temp;
    // Add the new argument
    current_cmd->cmd_arr[arg_count] = ft_strdup(arg_value);
    if (current_cmd->cmd_arr[arg_count] == NULL) 
        return (1); // Memory allocation error

    // Null terminate the array
    current_cmd->cmd_arr[arg_count + 1] = NULL;
    
    return (0); // Success
}

int handle_redirection(t_cmd *current_cmd, t_token **current, int current_type)
{
    if (!current_cmd || !current || !(*current)) return (1);

    *current = (*current)->next;
    
    if (!(*current) || !(*current)->value) {
        error(ERR_PARS, "\n"); // Ensure your error handling is consistent with the rest of your application
        return (2);
    }

    if (current_type == TOKEN_REDIRECT_OUT_APPEND) {
        current_cmd->redirection_append = ft_strdup((*current)->value);
        if (!current_cmd->redirection_append) return (1);
    } else if (current_type == TOKEN_REDIRECT_OUT) {
        current_cmd->output = ft_strdup((*current)->value);
        if (!current_cmd->output) return (1);
    }
    
    return (0); // Success
}


int handle_environment_variable(t_cmd *current_cmd, char *value)
{
    if (current_cmd == NULL) {
        return (1); // Return an error if there is no command to attach the environment variable to.
    }
    
    int env_var_count = 0;
    while (current_cmd->env_vars && current_cmd->env_vars[env_var_count] != NULL) {
        env_var_count++;
    }
    
    char **temp = realloc(current_cmd->env_vars, sizeof(char *) * (env_var_count + 2));
    if (!temp) {
        return 1; // Return an error code for memory allocation failure.
    }
    
    current_cmd->env_vars = temp;
    current_cmd->env_vars[env_var_count] = ft_strdup(value);
    if (!current_cmd->env_vars[env_var_count]) {
        return 1; // Return an error code for memory allocation failure.
    }
    
    current_cmd->env_vars[env_var_count + 1] = NULL; // NULL terminate the array.
    
    // Optional: For debugging purposes.
    printf("Parser env_var: %s\n", current_cmd->env_vars[env_var_count]);
    
    return 0; // Success
}