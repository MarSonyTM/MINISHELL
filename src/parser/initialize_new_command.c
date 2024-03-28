#include "../../inc/minishell.h"

// Function to resolve the path of a command
t_cmd *initialize_new_command(t_cmd **cmd, t_token *current_token, t_env *env) {
    t_cmd *new_cmd = new_command(cmd); // Create and link a new command
    if (!new_cmd) return NULL; // Handle failure

    // Allocate space for command name and NULL terminator
    new_cmd->cmd_arr = malloc(sizeof(char *) * 2);
    if (!new_cmd->cmd_arr) return NULL; // Handle failure

    new_cmd->cmd_arr[0] = ft_strdup(current_token->value); // Copy command/builtin name
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