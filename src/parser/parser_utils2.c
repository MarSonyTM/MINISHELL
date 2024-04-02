#include "../../inc/minishell.h"

// Adds the exit status token to the command's argument list.
int handle_exit_status_token(t_cmd *current_cmd, char *value, int *arg_count) {
    if (!current_cmd) return 1; // Ensure there is a current command to work with.

    // Increase the size of the command's argument array to accommodate the new argument.
    current_cmd->cmd_arr = realloc(current_cmd->cmd_arr, sizeof(char *) * (*arg_count + 2));
    if (!current_cmd->cmd_arr) return 1; // Check for allocation failure.

    // Add the exit status token as a new argument.
    current_cmd->cmd_arr[*arg_count] = ft_strdup(value);
    if (!current_cmd->cmd_arr[*arg_count]) return 1; // Check for strdup failure.

    // Increment the argument count and ensure the array is NULL-terminated.
    *arg_count += 1;
    current_cmd->cmd_arr[*arg_count] = NULL;

    return 0; // Success.
}