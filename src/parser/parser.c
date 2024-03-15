#include "../../inc/minishell.h"

char *resolve_command_path(char *command) 
{
    char *path = getenv("PATH"); // Get the PATH environment variable value
    char *pathCopy = ft_strdup(path); // Duplicate since strtok modifies the string
    char *dir = ft_strtok(pathCopy, ":");
    size_t commandLen = ft_strlen(command);

    while (dir != NULL) 
    {
        size_t dirLen = ft_strlen(dir);
        char *fullPath = malloc(dirLen + commandLen + 2); // For '/' and '\0'
        if (fullPath == NULL) {
            // Handle memory allocation failure
            free(pathCopy);
            return NULL;
        }
        
        ft_strcpy(fullPath, dir);
        fullPath[dirLen] = '/'; // Append '/'
        ft_strcpy(fullPath + dirLen + 1, command); // Append command
        if (access(fullPath, X_OK) == 0) 
        {
            free(pathCopy);
            return fullPath; // Command found
        }
        free(fullPath); // Free the allocated memory for fullPath
        dir = ft_strtok(NULL, ":");
    }
    free(pathCopy);  
    return NULL; // Command not found
}

// Function to add a new command to the list
t_cmd *new_cmd(t_cmd **cmd)
{
    t_cmd *new_cmd = malloc(sizeof(t_cmd));
    if (!new_cmd)
        return (NULL); // Error handling for malloc failure
    new_cmd->cmd_path = NULL;
    new_cmd->cmd_arr = malloc(sizeof(char *) * 2); // Initial size for command + NULL
    new_cmd->cmd_arr[0] = NULL; // Initialize to NULL for safety
    new_cmd->input = NULL;
    new_cmd->exit_status_token = NULL;
    new_cmd->env_vars = malloc(sizeof(char *) * 1); // Initial size for NULL
    new_cmd->env_vars[0] = NULL;
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
void parse(t_token *tokens, t_cmd **cmd) 
{
	t_cmd *current_cmd = NULL;
	int arg_count = 0; // To keep track of the number of arguments

	 
	t_token *current = tokens;
	while (current != NULL) 
	{
		if (current->type == TOKEN_BUILTIN) 
		{
			current_cmd = new_cmd(cmd); // Create a new command
			current_cmd->cmd_arr[0] = ft_strdup(current->value); // Command name
			current_cmd->cmd_arr[1] = NULL; // NULL terminate the array
			arg_count = 1; // Reset argument count for the new command
		}
		else if (current->type == TOKEN_COMMAND)
		{
			current_cmd = new_cmd(cmd); // Create a new command
			current_cmd->cmd_arr[0] = ft_strdup(current->value); // Command name
			current_cmd->cmd_arr[1] = NULL; // NULL terminate the array
			arg_count = 1; // Reset argument count for the new command
			char *cmd_path = resolve_command_path(current->value); // Resolve the command's path
			current_cmd->cmd_path = cmd_path; // Set the command's path
			if (cmd_path == NULL) 
			{
				// Handle command not found error
				printf("Error: Command not found\n");
				free_cmds(cmd);
				free(tokens);
				return ;
				// Clean up and exit or return an error
			}
		} 
		else if (current->type == TOKEN_ARG && current_cmd != NULL) 
		{
			arg_count++;
			current_cmd->cmd_arr = realloc(current_cmd->cmd_arr, sizeof(char *) * (arg_count + 1)); // Resize for new arg
			if (!current_cmd->cmd_arr) 
			{
				// Handle realloc failure
				printf("Error: Memory allocation failed\n");
				free_cmds(cmd);
				free(tokens);    
				return ;
				// Clean up and exit or return an error
			}
			current_cmd->cmd_arr[arg_count - 1] = ft_strdup(current->value); // Add the new argument
			current_cmd->cmd_arr[arg_count] = NULL; // NULL terminate the array
		} 
		else if (current->type == TOKEN_INPUT && current_cmd != NULL) 
		{
			current_cmd->input = ft_strdup(current->value);
		} 
		else if (current->type == TOKEN_REDIRECT_OUT || current->type == TOKEN_REDIRECT_OUT_APPEND || current->type == TOKEN_REDIRECT_IN) 
{
    // Store the current token type before moving to the next token
    int current_type = current->type;

    current = current->next;
    
if (current == NULL || current->value == NULL) 
{
    if (current_type == TOKEN_REDIRECT_OUT_APPEND) 
        printf("Error: Expected a file after >>\n");
    else if (current_type == TOKEN_REDIRECT_OUT) 
        printf("Error: Expected a file after >\n");
    else if (current_type == TOKEN_REDIRECT_IN) 
        printf("Error: Expected a file after <\n");
    else 
        printf("Error: Expected a file after >\n");
    free_cmds(cmd);
    return ;
}
    if (current_cmd != NULL) 
    {
        // If it was a double redirection out, set the redirection_append field
        if (current_type == TOKEN_REDIRECT_OUT_APPEND) 
        {
            // Set the output file for the command
            current_cmd->redirection_append = ft_strdup(current->value);
            printf("Parser redirection_append: %s\n", current_cmd->redirection_append); // Debugging
        }
        else // It was a single redirection out
        {
            // Set the output file for the command
            current_cmd->output = ft_strdup(current->value);
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
        printf("Error: Expected a delimiter after <<\n");
        free_cmds(cmd);
        return ;
    }

    char *delimiter = current->value; // Get the delimiter from the token value
    char *input_buffer = NULL;
    char *heredoc_input = NULL;

    // Read input line by line until the delimiter is encountered
    while (1) 
    {
        printf("> ");
        // Read input using readline
        input_buffer = readline(NULL);
        if (!input_buffer) 
        {
            // Error reading input
            printf("Error reading input\n");
            free_cmds(cmd);
            return;
        }
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
        } 
        else 
        {
            char *tmp = ft_strjoin(heredoc_input, "\n");
            free(heredoc_input);
            heredoc_input = ft_strjoin(tmp, input_buffer);
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
				{
					// Handle realloc failure
					printf("Error: Memory allocation failed\n");
					free_cmds(cmd);
					free(tokens);    
					return ;
					// Clean up and exit or return an error
				}
				current_cmd->cmd_arr[arg_count - 1] = ft_strdup(",");
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
            // Add the env_var to the array env_var
            int env_var_count = 0;
            while (current_cmd->env_vars[env_var_count] != NULL) 
            {
                env_var_count++;
            }
            current_cmd->env_vars = realloc(current_cmd->env_vars, sizeof(char *) * (env_var_count + 2)); // Resize for new env_var
            if (!current_cmd->env_vars) 
            {
                // Handle realloc failure
                printf("Error: Memory allocation failed\n");
                free_cmds(cmd);
                free(tokens);    
                return ;
                // Clean up and exit or return an error
            }
            current_cmd->env_vars[env_var_count] = ft_strdup(current->value);
            current_cmd->env_vars[env_var_count + 1] = NULL; // NULL terminate the array
            printf("Parser env_var: %s\n", current_cmd->env_vars[env_var_count]);
        }
        else if (current->type == TOKEN_EXIT_STATUS)
        {
            current_cmd->exit_status_token = ft_strdup(current->value);
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
				printf("Error: Expected a command after |\n");
				free_cmds(cmd);
				return;
			}

			// Create a new command structure for the next command in the pipeline
			current_cmd = new_cmd(cmd);
			current_cmd->cmd_arr[0] = ft_strdup(current->value); // Set the command for the new command structure
			current_cmd->cmd_arr[1] = NULL; // NULL terminate the array

			// Resolve the command's path
			char *cmd_path = resolve_command_path(current->value);
			current_cmd->cmd_path = cmd_path;

			// Handle command not found error
			if (cmd_path == NULL) 
			{
				printf("Error: Command not found\n");
				free_cmds(cmd);
				free(tokens);
				return;
			}
			// Reset argument count for the new command
			arg_count = 1;
		}
		current = current->next;
	}
} 