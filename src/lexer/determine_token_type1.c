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

// Helper function to process redirection append tokens
int process_redirect_out_append(char **buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, int *i, int inQuote) {
    if (*bufIndex > 0) {
        (*buffer)[*bufIndex] = '\0'; // Null-terminate the current token
        if (add_token(*tokens, determine_token_type(*buffer, inQuote, env, TokenCount), ft_strdup(*buffer)) == 1) {
            // Handle error if adding the token fails
            return (1); // Optionally, handle error
        }
        *bufIndex = 0; // Reset buffer index for the next token
        (*TokenCount)++;
    }
    // Add the redirect out append token
    if (add_token(*tokens, TOKEN_REDIRECT_OUT_APPEND, ft_strdup(">>")) == 1) {
        // Optionally, handle error
        return (1); // Optionally, handle error
    }
    (*TokenCount)++;
    (*i)++; // Move past the second '>'
    return (0);
}
