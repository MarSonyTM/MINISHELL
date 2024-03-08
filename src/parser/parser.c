#include "../../inc/minishell.h"

char *resolve_command_path(char *command) 
{
    char *path = getenv("PATH"); // Get the PATH environment variable value
    char *pathCopy = strdup(path); // Duplicate since strtok modifies the string
    char *dir = strtok(pathCopy, ":");

    while (dir != NULL) 
    {
        char *fullPath = malloc(strlen(dir) + strlen(command) + 2); // For '/' and '\0'
        sprintf(fullPath, "%s/%s", dir, command);
        if (access(fullPath, X_OK) == 0) 
        {
            free(pathCopy);
            return fullPath; // Command found
        }
        free(fullPath);
        dir = strtok(NULL, ":");
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
    int append_mode = 0; // Flag to indicate append mode for output redirection

    for (t_token *current = tokens; current; current = current->next) 
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
        else if (current->type == TOKEN_REDIRECT_OUT || current->type == TOKEN_DOUBLE_REDIRECT_OUT) 
        {
    if (current_cmd != NULL) 
    {
        // Set the output file for the command
        current_cmd->output = ft_strdup(current->value);
    }
    // If it's a double redirection out, set append mode flag
    if (current->type == TOKEN_DOUBLE_REDIRECT_OUT) 
    {
       append_mode = 1; 
    }
}
        else if (current->type == TOKEN_HEREDOC && current_cmd != NULL) 
        {
            // Handle here-document input
            // Logic to capture input until delimiter is encountered
            // Store captured input as command's stdin
        }
        else if (current->type == TOKEN_COMMA)
        {
            // Handle comma based on shell's syntax rules
            // Parse or ignore based on the role of commas in the shell
        }
        else if (current->type == TOKEN_ENV_VAR || current->type == TOKEN_EXIT_STATUS)
        {
            // Mark these tokens for later expansion
            // Process them during expansion or execution phases
        }
        else if (current->type == TOKEN_PIPE)
        {
            // When a pipe is encountered, prepare for a new command on the next iteration
            current_cmd = NULL; // This will trigger a new t_cmd creation on next command token
        }
        // Add handling for other types as necessary
    }
    
}