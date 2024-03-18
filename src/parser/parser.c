#include "../../inc/minishell.h"

char *resolve_command_path(char *command) 
{
    char *path = getenv("PATH"); // Get the PATH environment variable value
    char *pathCopy = ft_strdup(path); // Duplicate since strtok modifies the string
    if (!pathCopy)
        return (NULL); // Handle memory allocation failure
    char *dir = ft_strtok(pathCopy, ":");
    size_t commandLen = ft_strlen(command);

    while (dir != NULL) 
    {
        size_t dirLen = ft_strlen(dir);
        char *fullPath = malloc(dirLen + commandLen + 2); // For '/' and '\0'
        if (fullPath == NULL) {
            // Handle memory allocation failure
            free(pathCopy);
            return (NULL);
        }
        
        ft_strcpy(fullPath, dir);
        fullPath[dirLen] = '/'; // Append '/'
        ft_strcpy(fullPath + dirLen + 1, command); // Append command
        if (access (fullPath, F_OK) == 0 && access(fullPath, X_OK) == 0) 
        {
            free(pathCopy);
            return fullPath; // Command found
        }
        free(fullPath); // Free the allocated memory for fullPath
        dir = ft_strtok(NULL, ":");
    }
    free(pathCopy);  
    error(ERR_CMD, command); // Command not found
    return (NULL); // Command not found
}

// Function to add a new command to the list
t_cmd *new_command(t_cmd **cmd)
{
    t_cmd *new_cmd = malloc(sizeof(t_cmd));
    if (!new_cmd)
        return (NULL); // Error handling for malloc failure
    new_cmd->cmd_path = NULL;
    new_cmd->cmd_arr = malloc(sizeof(char *) * 2); // Initial size for command + NULL
    if (!new_cmd->cmd_arr)
    {
        free(new_cmd);
        return (NULL); // Error handling for malloc failure
    }
    new_cmd->cmd_arr[0] = NULL; // Initialize to NULL for safety
    new_cmd->input = NULL;
    new_cmd->exit_status_token = NULL;
    new_cmd->env_var = NULL;
    new_cmd->output = NULL;
    new_cmd->exit_status = 0;
    new_cmd->next = NULL;

    if (*cmd == NULL)
        *cmd = new_cmd; // If it's the first command
    else
    {
        // Append to the end of the list
        t_cmd *last = *cmd;
        while (last->next != NULL)
            last = last->next;
        last->next = new_cmd;
    }
    return (new_cmd);
}

// Main parse function
int parse(t_token *tokens, t_cmd **cmd)
{
	t_cmd *current_cmd = NULL;
	int arg_count = 0; // To keep track of the number of arguments
	 
	t_token *current = tokens;
	while (current != NULL) 
	{
		if (current->type == TOKEN_BUILTIN) 
		{
			current_cmd = new_command(cmd); // Create a new command
            if (!current_cmd)
                return (1);
			current_cmd->cmd_arr[0] = ft_strdup(current->value); // Command name
            if (!current_cmd->cmd_arr[0])
                return (1);
			current_cmd->cmd_arr[1] = NULL; // NULL terminate the array
			arg_count = 1; // Reset argument count for the new command
		}
		else if (current->type == TOKEN_COMMAND)
		{
			current_cmd = new_command(cmd); // Create a new command
            if (!current_cmd) 
                return (1);
			current_cmd->cmd_arr[0] = ft_strdup(current->value); // Command name
            if (!current_cmd->cmd_arr[0])
                return (1);
			current_cmd->cmd_arr[1] = NULL; // NULL terminate the array
			arg_count = 1; // Reset argument count for the new command
			char *cmd_path = resolve_command_path(current->value); // Resolve the command's path
            if (!cmd_path) 
                return (1);
			current_cmd->cmd_path = cmd_path; // Set the command's path
			if (cmd_path == NULL)
                return (1);
		} 
		else if (current->type == TOKEN_ARG && current_cmd != NULL) 
		{
			arg_count++;
			current_cmd->cmd_arr = realloc(current_cmd->cmd_arr, sizeof(char *) * (arg_count + 1)); // Resize for new arg
			if (!current_cmd->cmd_arr) 
                return (1);
			current_cmd->cmd_arr[arg_count - 1] = ft_strdup(current->value); // Add the new argument
            if (!current_cmd->cmd_arr[arg_count - 1])
                return (1);
			current_cmd->cmd_arr[arg_count] = NULL; // NULL terminate the array
		} 
		else if (current->type == TOKEN_INPUT && current_cmd != NULL) 
		{
			current_cmd->input = ft_strdup(current->value);
            if (!current_cmd->input)
            {
                return (1);
            }
		} 
		else if (current->type == TOKEN_REDIRECT_OUT || current->type == TOKEN_REDIRECT_OUT_APPEND) 
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
        }
		else if (current->type == TOKEN_HEREDOC) 
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
            
        }
		else if (current->type == TOKEN_COMMA)
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
		}
		else if (current->type == TOKEN_ENV_VAR || current->type == TOKEN_EXIT_STATUS)
        {
            // Mark these tokens for later expansion
            if (current_cmd != NULL)
            {
                if (current->type == TOKEN_ENV_VAR)
                {
                    current_cmd->env_var = ft_strdup(current->value);
                    if (!current_cmd->env_var)
                        return (1);
                    printf("Parser env_var: %s\n", current_cmd->env_var);
                }
                else if (current->type == TOKEN_EXIT_STATUS)
                {
                    current_cmd->exit_status_token = ft_strdup(current->value);
                    if (!current_cmd->exit_status_token)
                        return (1);
                    printf("Parser exit_status_token: %s\n", current_cmd->exit_status_token);
                }
            }
        }
		else if (current->type == TOKEN_PIPE)
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
			char *cmd_path = resolve_command_path(current->value);
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