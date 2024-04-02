#include "../../inc/minishell.h"

int handleBuiltinOrCommand(t_cmd **cmd, t_token *current, t_env *env, t_cmd **current_cmd , int *arg_count)
{
    *current_cmd = initialize_new_command(cmd, current, env);
    if (!(*current_cmd)) return 1; // Handle memory allocation error
    *arg_count = 1; // Reset argument count for new command
    return 0; // Success
}

int handleArgument(t_cmd *current_cmd, t_token *current)
{
    if (current_cmd == NULL) return 0; // Skip if no current command
    if (add_argument_to_command(current_cmd, current->value) != 0) return 1; // Handle error
    return 0; // Success
}

int handleInput(t_cmd *current_cmd, t_token *current)
{
    if (current_cmd == NULL) return 0; // Skip if no current command
    current_cmd->input = ft_strdup(current->value);
    if (!current_cmd->input) return 1; // Handle error
    return 0; // Success
}

int handleRedirection(t_cmd *current_cmd, t_token **current)
{
    if (current_cmd == NULL) return (0); // Skip if no current command
    int result = handle_redirection(current_cmd, current, (*current)->type);
    if (result != 0) return result;
    return 0; // Success
}

int handleHeredoc(t_cmd **current_cmd, t_token **current) {
    if (!(*current)->next) {
        error(ERR_PARS, "\n"); // Set the specific error
        return 2; // Indicate a specific error condition
    }
    *current = (*current)->next; // Move to the delimiter token

    char *heredoc_input = handle_heredoc(current);
    if (!heredoc_input) {
        // Assuming handle_heredoc sets an appropriate error internally
        return 1; // General error for heredoc input failure
    }

    if (*current_cmd) {
        (*current_cmd)->input = heredoc_input;
    } else {
        free(heredoc_input); // Cleanup heredoc input if no command to attach it to
    }

    return 0; // Success
}


int handleComma(t_cmd *current_cmd, t_token *current)
{ 
    if (current_cmd == NULL) return 0; // Skip if no current command
    if (add_argument_to_command(current_cmd, current->value) != 0) return 1; // Handle error
    return 0; // Success
}

int handleExitStatus(t_cmd *current_cmd, t_token *current)
{
    if (current_cmd == NULL) return 0; // Skip if no current command
    current_cmd->exit_status_token = ft_strdup(current->value);
    if (!current_cmd->exit_status_token) return 1; // Handle error
    return 0; // Success
}

