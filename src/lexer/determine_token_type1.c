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
    ft_strcpy(fullPath, dir);
    ft_strcat(fullPath, "/");
    ft_strcat(fullPath, token);
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


int process_input_char(char currentChar, char *input,  char **buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, int *i, int *inQuote, bool *quote_error) {
    (void)quote_error;
    if (is_whitespace(currentChar) && *inQuote == 0) {
        process_whitespace(*buffer, bufIndex, tokens, TokenCount, env);
    } else if (currentChar == '|') {
        process_pipe(*buffer, bufIndex, tokens, TokenCount, env);
    } else if (currentChar == '\'' || currentChar == '\"') {
        process_quotes(currentChar, buffer, bufIndex, inQuote);
    } else if (currentChar == ',' && *inQuote == 0) {
        process_comma(*buffer, bufIndex, tokens, TokenCount, env);
    } else if (currentChar == '<' && input[*i + 1] != '<' && *inQuote == 0) {
        process_single_char_redirection(currentChar, *buffer, bufIndex, tokens, TokenCount, env);
    } else if (currentChar == '>' && input[*i + 1] != '>' && *inQuote == 0) {
        process_double_char_redirection(currentChar, *buffer, bufIndex, tokens, TokenCount, env, i);
    } else if (currentChar == '$') {
        process_dollar_conditions(input, i, buffer, bufIndex, tokens, TokenCount, env, *inQuote);
    } else if (currentChar == '<' && input[*i + 1] == '<' && *inQuote == 0) {
        process_heredoc(buffer, bufIndex, tokens, TokenCount, env, i);
    } else if (currentChar == '>' && input[*i + 1] == '>' && *inQuote == 0) {
        process_redirect_out_append(buffer, bufIndex, tokens, TokenCount, env, i, *inQuote);
    } else {
        // Regular character, add to the buffer
        (*buffer)[(*bufIndex)++] = currentChar;
    }
    return 0; // Return 0 to indicate success
}
