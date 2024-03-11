#include "../../inc/minishell.h"


char *append_string(const char *str1, const char *str2) 
{
    size_t len1 = ft_strlen(str1);
    size_t len2 = ft_strlen(str2);

    // Allocate memory for the concatenated string
    char *result = malloc(len1 + len2 + 1);
    if (!result)
    {
        // Error handling for memory allocation failure
        return NULL;
    }

    // Copy the contents of str1 and str2 into the result buffer
    ft_strcpy(result, str1);
    ft_strcpy(result + len1, str2);

    return result;
}

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
        free(fullPath);
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
        else if (current->type == TOKEN_HEREDOC) 
        {
    // Advance to the next token and use its value as the delimiter
    current = current->next;
    if (current == NULL) {
        printf("Error: Expected a delimiter after <<\n");
        free_cmds(cmd);
        return;
    }

    char *delimiter = current->value; // Get the delimiter from the token value
    char *input_buffer = NULL;

    // Read input line by line until the delimiter is encountered
    while (1) 
    {
        printf("> ");
        // Read input using readline
        input_buffer = readline(NULL);
        if (!input_buffer) {
            // Error reading input
            printf("Error reading input\n");
            free_cmds(cmd);
            return;
        }

        // Strip newline from input_buffer
        input_buffer[ft_strcspn(input_buffer, "\n")] = '\0';

        if (ft_strcmp(input_buffer, delimiter) == 0) {
            // Delimiter encountered, stop reading input
            free(input_buffer);
            break;
        }
        free(input_buffer); // Free the input buffer
    }
}
        else if (current->type == TOKEN_COMMA)
{
    // Handle commas by treating them as part of the input
    // You can choose to ignore them or include them as part of arguments
    // For simplicity, let's assume we include them as part of arguments
    if (current_cmd != NULL  ) 
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
            // Process them during expansion or execution phases
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
	}
} 