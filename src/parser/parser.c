#include "../../inc/minishell.h"



// Main parse function
int parse(t_token *tokens, t_cmd **cmd, t_env *env)
{
	t_cmd *current_cmd = NULL;
	int arg_count = 0; // To keep track of the number of arguments
	 
	t_token *current = tokens;
	while (current != NULL) 
	{
		while (current != NULL) 
		{
        if (current->type == TOKEN_BUILTIN || current->type == TOKEN_COMMAND)
		{
            current_cmd = initialize_new_command(cmd, current, env);
            if (!current_cmd)
                return (1); // Handle memory allocation error
            arg_count = 1; // Reset argument count for new command
        }else if (current->type == TOKEN_ARG /*&& current_cmd != NULL*/) 
		{
			arg_count++;
			current_cmd->cmd_arr = realloc(current_cmd->cmd_arr, sizeof(char *) * (arg_count + 1)); // Resize for new arg
			if (!current_cmd->cmd_arr) 
                return (1);
			current_cmd->cmd_arr[arg_count - 1] = ft_strdup(current->value); // Add the new argument
            if (!current_cmd->cmd_arr[arg_count - 1])
                return (1);
			current_cmd->cmd_arr[arg_count] = NULL; // NULL terminate the array
		}else if (current->type == TOKEN_INPUT && current_cmd != NULL) 
		{
			current_cmd->input = ft_strdup(current->value);
            if (!current_cmd->input)
            {
                return (1);
            }
		}else if (current->type == TOKEN_REDIRECT_OUT || current->type == TOKEN_REDIRECT_OUT_APPEND) 
        {
            // Store the current token type before moving to the next token
            int current_type = current->type;

            current = current->next;
                
            if (current == NULL || current->value == NULL)
            {
                if (current_type == TOKEN_REDIRECT_OUT_APPEND) 
                    error(ERR_PARS, "\n");
                else
                    error(ERR_PARS, "\n");
                return (2);
            }
            if (current_cmd != NULL)
            {
                // If it was a double redirection out, set the redirection_append field
                if (current_type == TOKEN_REDIRECT_OUT_APPEND) 
                {
                    // Set the output file for the command
                    current_cmd->redirection_append = ft_strdup(current->value);
                    if (!current_cmd->redirection_append)
                        return (1);
                    printf("Parser redirection_append: %s\n", current_cmd->redirection_append); // Debugging
                }
                else // It was a single redirection out
                {
                    // Set the output file for the command
                    current_cmd->output = ft_strdup(current->value);
                    if (!current_cmd->output)
                        return (1);
                    printf("Parser output: %s\n", current_cmd->output); // Debugging
                }
            }
        }else if (current->type == TOKEN_HEREDOC) 
        {
            // Advance to the next token and use its value as the delimiter
            current = current->next;
            if (current == NULL) 
            {
                error(ERR_PARS, "\n");
                return (2);
            }

            char *delimiter = current->value; // Get the delimiter from the token value
            char *input_buffer = NULL;
            char *heredoc_input = NULL;

            // Read input line by line until the delimiter is encountered
            while (1) 
            {
                ft_putstr_fd("> ", 1);
                // Read input using readline
                input_buffer = readline(NULL);
                if (!input_buffer) 
                    return (1);
                // Strip newline from input_buffer
                input_buffer[ft_strcspn(input_buffer, "\n")] = '\0';

                if (ft_strcmp(input_buffer, delimiter) == 0) 
                {
                    // Delimiter encountered, stop reading input
                    free(input_buffer);
                    break;
                }
                // Concatenate the input to heredoc_input
                if (heredoc_input == NULL) 
                {
                    heredoc_input = ft_strdup(input_buffer);
                    if (!heredoc_input)
                    {
                        free(input_buffer);
                        return (1);
                    }
                } 
                else 
                {
                    char *tmp = ft_strjoin(heredoc_input, "\n");
                    if (!tmp)
                    {
                        free(input_buffer);
                        free(heredoc_input);
                        return (1);
                    }
                    free(heredoc_input);
                    heredoc_input = ft_strjoin(tmp, input_buffer);
                    if (!heredoc_input)
                    {
                        free(tmp);
                        free(input_buffer);
                        return (1);
                    }
                    free(tmp);
                }
                free(input_buffer); // Free the input buffer
            }
            // Store the input in the command structure
            if (current_cmd != NULL) 
            {
                current_cmd->input = heredoc_input;
            }
            printf("heredoc_input: %s\n", heredoc_input); // Debugging
            
        }else if (current->type == TOKEN_COMMA)
		{
			// Handle commas by treating them as part of the input
			if (current_cmd != NULL) 
			{
				arg_count++;
				current_cmd->cmd_arr = realloc(current_cmd->cmd_arr, sizeof(char *) * (arg_count + 1)); // Resize for new arg
				if (!current_cmd->cmd_arr)
					return (1);
				current_cmd->cmd_arr[arg_count - 1] = ft_strdup(",");
                if (!current_cmd->cmd_arr[arg_count - 1])
                    return (1);
				current_cmd->cmd_arr[arg_count] = NULL; // NULL terminate the array
			} 
		}else if (current->type == TOKEN_EXIT_STATUS)
        {
            // Store the exit status token in the command structure
            if (current_cmd != NULL) 
            {
                current_cmd->cmd_arr = realloc(current_cmd->cmd_arr, sizeof(char *) * (arg_count + 1)); // Resize for new arg
                current_cmd->cmd_arr[arg_count] = ft_strdup(current->value);
                if (!current_cmd->cmd_arr[arg_count])
                    return (1);
            }
        }else if (current->type == TOKEN_ENV_VAR)
        {
            int env_var_count = 0;
            while (current_cmd->env_vars[env_var_count] != NULL) 
            {
                env_var_count++;
            }
            current_cmd->env_vars = realloc(current_cmd->env_vars, sizeof(char *) * (env_var_count + 2)); // Resize for new env_var
            if (!current_cmd->env_vars) 
            {  
                return (1);
                // Clean up and exit or return an error
            }
            current_cmd->env_vars[env_var_count] = ft_strdup(current->value);
            if (!current_cmd->env_vars[env_var_count])
                return (1);
            current_cmd->env_vars[env_var_count + 1] = NULL; // NULL terminate the array
            printf("Parser env_var: %s\n", current_cmd->env_vars[env_var_count]);
 
        }else if (current->type == TOKEN_PIPE)
		{
			// Advance to the next token and use its value as the command for the new command structure
			current = current->next;
			if (current == NULL || (current->type != TOKEN_COMMAND && current->type != TOKEN_BUILTIN)) 
			{
                /* EDGE CASE! */
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
} 
    return (0);
}
