#include "../../inc/minishell.h"

bool is_command(char *token, t_env *env) 
{
    char *path = ft_getenv("PATH", env);
    if (!path) return (false);

    char *pathCopy = ft_strdup(path);
    char *dir = strtok(pathCopy, ":");

    while (dir) {
        char *fullPath = construct_full_path(dir, token);
        bool exists = (access(fullPath, F_OK | X_OK) == 0);
        free(fullPath);
        if (exists) {
            free(pathCopy);
            return (true);
        }
        dir = strtok(NULL, ":");
    }
    free(pathCopy);
    return (false);
}

char	*construct_full_path(char *dir, char *token) 
{
	size_t	fullPathLen;
	char	*fullPath;

	fullPathLen = strlen(dir) + strlen(token) + 2; // For '/' and '\0'
	fullPath = malloc(fullPathLen);
    if (!fullPath) 
	{
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    // Manually copy and concatenate strings
    strcpy(fullPath, dir);
    strcat(fullPath, "/");
    strcat(fullPath, token);
    return (fullPath);
}
