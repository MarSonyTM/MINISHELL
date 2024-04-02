#include "../../inc/minishell.h"

 
// Main parse function
int parse(t_token *tokens, t_cmd **cmd, t_env *env)
{
	t_cmd *current_cmd = NULL;
	int arg_count = 0; // To keep track of the number of arguments
	 
	t_token *current = tokens;
	
		while (current != NULL) 
		{
        if (current->type == TOKEN_BUILTIN || current->type == TOKEN_COMMAND)
		{
            current_cmd = initialize_new_command(cmd, current, env);
            if (!current_cmd)
                return (1); // Handle memory allocation error
            arg_count = 1; // Reset argument count for new command
        }
        else if (current->type == TOKEN_ARG /*&& current_cmd != NULL*/)
        {
            if (add_argument_to_command(current_cmd, current->value) != 0)
        return (1); // Handle error 
        }
        else if (current->type == TOKEN_INPUT && current_cmd != NULL) 
		{
			current_cmd->input = ft_strdup(current->value);
            if (!current_cmd->input)
                return (1);
		}
        else if (current->type == TOKEN_REDIRECT_OUT || current->type == TOKEN_REDIRECT_OUT_APPEND) 
        { 
            int result = handle_redirection(current_cmd, &current, current->type);
            if (result != 0) return (result);
        }
        else if (current->type == TOKEN_HEREDOC)
        {
                 current = current->next; // Move to the delimiter token
         if (!current)
         {
                 error(ERR_PARS, "\n");
                 return (2);
         }
     char *heredoc_input = handle_heredoc(&current);
    if (!heredoc_input) return (1); // Handle error, potentially due to readline failure
    if (current_cmd != NULL) {
        current_cmd->input = heredoc_input;
    } else 
        free(heredoc_input); // If there's no current command, cleanup
}

        else if (current->type == TOKEN_COMMA)
		{ 
            if (add_argument_to_command(current_cmd, current->value) != 0)
                return (1);     
		}
        else if (current->type == TOKEN_EXIT_STATUS)
        {
                int result = handle_exit_status_token(current_cmd, current->value, &arg_count);
                 if (result != 0) return result;
        }
        else if (current->type == TOKEN_ENV_VAR)
        {
             int result = handle_environment_variable(current_cmd, current->value);
             if (result != 0) return result; // Handle the error based on the returned code.
        }
        else if (current->type == TOKEN_PIPE)
		{
			// Advance to the next token and use its value as the command for the new command structure
			current = current->next;
			if (current == NULL || (current->type != TOKEN_COMMAND && current->type != TOKEN_BUILTIN)) 
			{
                error(ERR_PARS, "\n");
                return (2);
			}

			// Create a new command structure for the next command in the pipeline
			current_cmd = new_command(cmd);
            if (!current_cmd)
                return (1);
			current_cmd->cmd_arr[0] = ft_strdup(current->value); // Set the command for the new command structure
			if (!current_cmd->cmd_arr[0])
                return (1);
            current_cmd->cmd_arr[1] = NULL; // NULL terminate the array

			// Resolve the command's path
			char *cmd_path = resolve_command_path(current->value, env);
            if (!cmd_path) 
                return (1);
			current_cmd->cmd_path = cmd_path;
			// Reset argument count for the new command
			arg_count = 1;
		}
		current = current->next;
	}
    return (0);
} 
