#include "../../inc/minishell.h"

int process_heredoc(char **buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, int *i) {
    if (*bufIndex > 0) {
        (*buffer)[*bufIndex] = '\0'; // Null-terminate the current token
        if (add_token(*tokens, determine_token_type(*buffer, 0, env, TokenCount), strdup(*buffer)) == 1) {
            // Handle error
            return (1); // Exiting early may be needed depending on error handling strategy
        }
        *bufIndex = 0; // Reset buffer index for the next token
        (*TokenCount)++;
    }
    // Add the heredoc token
    if (add_token(*tokens, TOKEN_HEREDOC, strdup("<<")) == 1) {
        // Handle error
        return (1); // Exiting early may be needed depending on error handling strategy
    }
    (*TokenCount)++;
    (*i)++; // Move past the second '<'
    return (0);
}