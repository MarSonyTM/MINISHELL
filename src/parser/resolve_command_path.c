#include "../../inc/minishell.h"

char *resolve_command_path(char *command, t_env *env) 
{
    char *path = ft_getenv("PATH", env); // Get the PATH environment variable value
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
    new_cmd->env_vars = malloc(sizeof(char *) * 1); // Initial size for NULL
    if (!new_cmd->env_vars)
    {
        free(new_cmd->cmd_arr);
        free(new_cmd);
        return (NULL); // Error handling for malloc failure
    }
    new_cmd->env_vars[0] = NULL; // Initialize to NULL for safety
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