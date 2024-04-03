#include "../../inc/minishell.h"

// Adds the exit status token to the command's argument list.
int handle_exit_status_token(t_cmd *current_cmd, char *value, int *arg_count)
{
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

t_cmd *handle_pipe_token(t_token **current, t_cmd **cmd, t_env *env, int *arg_count)
{
    // Advance to the next token and use its value as the command for the new command structure
    *current = (*current)->next;
    if (*current == NULL || ((*current)->type != TOKEN_COMMAND && (*current)->type != TOKEN_BUILTIN)) 
        return (error(ERR_PARS, "\n"), NULL);
    // Create a new command structure for the next command in the pipeline
    t_cmd *current_cmd = new_command(cmd);
    if (!current_cmd)
        return (NULL);
    current_cmd->cmd_arr[0] = ft_strdup((*current)->value); // Set the command for the new command structure
    if (!current_cmd->cmd_arr[0])
        return (NULL);
    current_cmd->cmd_arr[1] = NULL; // NULL terminate the array
    // Resolve the command's path
    char *cmd_path = resolve_command_path((*current)->value, env);
    if (!cmd_path) 
        return (NULL);
    current_cmd->cmd_path = cmd_path;
    // Reset argument count for the new command
    *arg_count = 1;
    return (current_cmd);
}

int process_tokens(t_token *tokens, t_cmd **cmd, t_env *env)
{
    t_cmd *current_cmd = NULL;
    int arg_count = 0; // Initialization moved inside the function
    t_token *current = tokens;

    while (current != NULL) 
		{
        if (current->type == TOKEN_BUILTIN || current->type == TOKEN_COMMAND)
            handle_builtin_or_command(cmd, current, env, &current_cmd, &arg_count);
        else if (current->type == TOKEN_ARG /*&& current_cmd != NULL*/)
            handle_argument(current_cmd, current);
        else if (current->type == TOKEN_INPUT && current_cmd != NULL) 
			handle_input(current_cmd, current);            
        else if (current->type == TOKEN_REDIRECT_OUT || current->type == TOKEN_REDIRECT_OUT_APPEND) 
            handle_parser_redirection(current_cmd, &current);
        else if (current->type == TOKEN_HEREDOC)
            handle_parser_heredoc(&current_cmd, &current);
        else if (current->type == TOKEN_COMMA)
            handle_comma(current_cmd, current);    
        else if (current->type == TOKEN_EXIT_STATUS)
            handle_exit_status_token(current_cmd, current->value, &arg_count);
        else if (current->type == TOKEN_ENV_VAR)
            handle_environment_variable(current_cmd, current->value);
        else if (current->type == TOKEN_PIPE)
        {
              current_cmd = handle_pipe_token(&current, cmd, env, &arg_count);
              if (!current_cmd) return (1); // Handle error
        }
		current = current->next;
	}
    return (0); 
}

int handle_comma(t_cmd *current_cmd, t_token *current)
{ 
    if (current_cmd == NULL) return 0; // Skip if no current command
    if (add_argument_to_command(current_cmd, current->value) != 0) return 1; // Handle error
    return 0; // Success
}

int handle_cxit_status(t_cmd *current_cmd, t_token *current)
{
    if (current_cmd == NULL) return 0; // Skip if no current command
    current_cmd->exit_status_token = ft_strdup(current->value);
    if (!current_cmd->exit_status_token) return 1; // Handle error
    return 0; // Success
}