#include "../../inc/minishell.h"

t_token_type search_for_commands_in_path(char *token) 
{
    char *path, *pathCopy, *dir;
    char *fullPath;
    size_t fullPathLen;
    int found;

    path = getenv("PATH");
    if (!path) return TOKEN_ARG; // Early return if PATH is not set

    pathCopy = strdup(path); // Use ft_strdup if available
    if (!pathCopy) return TOKEN_ERROR;

    dir = strtok(pathCopy, ":"); // Use ft_strtok if available
    found = 0;
    while (dir != NULL && !found) {
        fullPathLen = strlen(dir) + strlen(token) + 2; // +2 for '/' and '\0'
        fullPath = malloc(fullPathLen);
        if (!fullPath) {
            free(pathCopy);
            return TOKEN_ERROR;
        }
        strcpy(fullPath, dir); // Use ft_strcpy if available
        if (fullPath[strlen(fullPath) - 1] != '/') strcat(fullPath, "/"); // Use ft_strcat if available
        strcat(fullPath, token);
        if (access(fullPath, F_OK) == 0 && access(fullPath, X_OK) == 0) found = 1;
        free(fullPath);
        dir = strtok(NULL, ":");
    }
    free(pathCopy);
    if (found) 
        return TOKEN_COMMAND;
    else 
        return TOKEN_ARG;
}
