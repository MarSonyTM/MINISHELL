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
                return (error(ERR_PARS, "\n"), 2); // Handle error
            char *heredoc_input = handle_heredoc(&current);
        if (!heredoc_input) return (1); // Handle error, potentially due to readline failure
        if (current_cmd != NULL) 
            current_cmd->input = heredoc_input;
        else 
            free(heredoc_input); // If there's no current command, cleanup
        }
        else if (current->type == TOKEN_COMMA)
		{ 
            if (add_argument_to_command(current_cmd, current->value) != 0)
                return (1);     
		}
        else if (current->type == TOKEN_EXIT_STATUS)
        {
               if (handle_exit_status_token(current_cmd, current->value, &arg_count) != 0)
                return (1); // Handle error
        }
        else if (current->type == TOKEN_ENV_VAR)
        {
             if (handle_environment_variable(current_cmd, current->value) != 0)
                return (1); // Handle error   
        }
        else if (current->type == TOKEN_PIPE)
        {
              current_cmd = handle_pipe_token(&current, cmd, env, &arg_count);
        if (!current_cmd)
        return (1);
        }
		current = current->next;
	}
    return (0);
} 
