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

// Helper function to process single character redirection "<"
void process_single_redirect_in(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, int inQuote) {
    if (*bufIndex > 0) {
        buffer[*bufIndex] = '\0'; // Null-terminate the current token
        // Add the current token before adding the redirect in token
        if (add_token(*tokens, determine_token_type(buffer, inQuote, env, TokenCount), ft_strdup(buffer)) == 1) 
            return; // Handle error appropriately
        *bufIndex = 0; // Reset buffer index for the next token
    }
    // Add the single character redirect in token
    if (add_token(*tokens, TOKEN_REDIRECT_IN, ft_strdup("<")) == 1) 
        return; // Handle error appropriately
    (*TokenCount)++;
}

// Finalization logic for the lexer.
int finalize_lexer(char **buffer, int bufIndex, t_token ***tokens, int *TokenCount, int inQuote, bool quote_error, t_env *env) {
    if (bufIndex > 0 && !quote_error) {
        if (inQuote != 0) {
            error(ERR_QUOT, NULL);
            quote_error = true;
        } else {
            (*buffer)[bufIndex] = '\0';
            if (add_token(*tokens, determine_token_type(*buffer, inQuote, env, TokenCount), ft_strdup(*buffer)) == 1) {
                free(*buffer);
                return 2;
            }
            (*TokenCount)++;
        }
    }
    if (quote_error) {
        free(*buffer);
        return 2; // Return indicating an error occurred
    }
    free(*buffer); // Free the buffer now that we're done with it
    return 0; // Return indicating success
}