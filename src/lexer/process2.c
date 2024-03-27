#include "../../inc/minishell.h"

// Helper function to process a single character redirection '>'
void process_single_redirect_out(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env) {
    if (*bufIndex > 0) {
        buffer[*bufIndex] = '\0'; // Null-terminate the current token
        // Add the current token before adding the redirect out token
        if (add_token(*tokens, determine_token_type(buffer, 0, env, TokenCount), ft_strdup(buffer)) == 1) 
            return; // Handle error appropriately
        *bufIndex = 0; // Reset buffer index for the next token
    }
    // Add the single character redirect out token
    if (add_token(*tokens, TOKEN_REDIRECT_OUT, ft_strdup(">")) == 1) 
        return; // Handle error appropriately
    (*TokenCount)++;
}

 // Helper function to process double character redirection ">>"
void process_redirect_out_append(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, int *i, int inQuote) {
    if (*bufIndex > 0) {
        buffer[*bufIndex] = '\0'; // Null-terminate the current token
        // Add the current token before adding the redirect out append token
        if (add_token(*tokens, determine_token_type(buffer, inQuote, env, TokenCount), ft_strdup(buffer)) == 1) 
            return; // Handle error appropriately
        *bufIndex = 0; // Reset buffer index for the next token
    }
    // Add the double character redirect out append token
    if (add_token(*tokens, TOKEN_REDIRECT_OUT_APPEND, ft_strdup(">>")) == 1) 
        return; // Handle error appropriately
    (*TokenCount)++;
    (*i)++; // Move past the second '>' character
}