#include "../../inc/minishell.h"

// Helper function to process a single character redirection '>'
int    process_single_redirect_out(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env)
{
    if (*bufIndex > 0) 
    {
        buffer[*bufIndex] = '\0'; // Null-terminate the current token
        // Add the current token before adding the redirect out token
        if (add_token(*tokens, determine_token_type(buffer, 0, env, TokenCount), ft_strdup(buffer)) == 1) 
            return (1); // Handle error appropriately
        *bufIndex = 0; // Reset buffer index for the next token
    }
    // Add the single character redirect out token
    if (add_token(*tokens, TOKEN_REDIRECT_OUT, ft_strdup(">")) == 1) 
        return (1); // Handle error appropriately
    (*TokenCount)++;
    return (0); // Return 0 to indicate success
}

 // Helper function to process double character redirection ">>"
int process_redirect_out_append(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, int *i, int inQuote)
{
    if (*bufIndex > 0)
    {
        buffer[*bufIndex] = '\0'; // Null-terminate the current token
        // Add the current token before adding the redirect out append token
        if (add_token(*tokens, determine_token_type(buffer, inQuote, env, TokenCount), ft_strdup(buffer)) == 1) 
            return (1); // Handle error appropriately
        *bufIndex = 0; // Reset buffer index for the next token
    }
    // Add the double character redirect out append token
    if (add_token(*tokens, TOKEN_REDIRECT_OUT_APPEND, ft_strdup(">>")) == 1) 
        return (1); // Handle error appropriately
    (*TokenCount)++;
    (*i)++; // Move past the second '>' character
    return (0); // Return 0 to indicate success
}

// Helper function to process single character redirection "<"
int process_single_redirect_in(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, int inQuote)
{
    if (*bufIndex > 0)
    {
        buffer[*bufIndex] = '\0'; // Null-terminate the current token
        // Add the current token before adding the redirect in token
        if (add_token(*tokens, determine_token_type(buffer, inQuote, env, TokenCount), ft_strdup(buffer)) == 1) 
            return (1); // Handle error appropriately
        *bufIndex = 0; // Reset buffer index for the next token
    }
    // Add the single character redirect in token
    if (add_token(*tokens, TOKEN_REDIRECT_IN, ft_strdup("<")) == 1) 
        return (1); // Handle error appropriately
    (*TokenCount)++;
    return (0); // Return 0 to indicate success
}

// Proposed helper function for the main processing loop of the lexer
int process_input_loop(char *input, char **buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, int *i, int *inQuote, bool *quote_error) {
    char currentChar;

    while ((currentChar = input[*i]) != '\0' && !(*quote_error))
    {
        if (is_whitespace(currentChar) && *inQuote == 0)
            process_whitespace(*buffer, bufIndex, tokens, TokenCount, env);
        else if (currentChar == '|')
            process_pipe(*buffer, bufIndex, tokens, TokenCount, env);
        else if (currentChar == '\'' || currentChar == '\"')
            process_quotes(currentChar, buffer, bufIndex, inQuote);
        else if (currentChar == ',' && *inQuote == 0)
            process_comma(*buffer, bufIndex, tokens, TokenCount, env);
        else if (currentChar == '<' && input[*i + 1] != '<' && *inQuote == 0)
            process_single_redirect_in(*buffer, bufIndex, tokens, TokenCount, env, *inQuote);
        else if (currentChar == '>' && input[*i + 1] != '>' && *inQuote == 0)
            process_single_redirect_out(*buffer, bufIndex, tokens, TokenCount, env);
        else if (currentChar == '$')
            process_dollar_conditions(input, i, buffer, bufIndex, tokens, TokenCount, env, *inQuote);
        else if (currentChar == '<' && input[*i + 1] == '<' && *inQuote == 0)
            process_heredoc(buffer, bufIndex, tokens, TokenCount, env, i);
        else if (currentChar == '>' && input[*i + 1] == '>' && *inQuote == 0)
            process_redirect_out_append(*buffer, bufIndex, tokens, TokenCount, env, i, *inQuote);
        else 
            (*buffer)[(*bufIndex)++] = currentChar; // Regular character, add to the buffer
        (*i)++;
    }

    return (0); // Return 0 to indicate success
}

// Finalization logic for the lexer.
int finalize_lexer(char **buffer, int bufIndex, t_token ***tokens, int *TokenCount, int inQuote, bool quote_error, t_env *env)
{
    if (bufIndex > 0 && !quote_error) 
    {
        if (inQuote != 0) 
        {
            error(ERR_QUOT, NULL);
            quote_error = true;
        }
        else 
        {
            (*buffer)[bufIndex] = '\0';
            if (add_token(*tokens, determine_token_type(*buffer, inQuote, env, TokenCount), ft_strdup(*buffer)) == 1)
                return (free(*buffer), 1);
            (*TokenCount)++;
        }
    }
    if (quote_error)
        return (free(*buffer), 2);
    free(*buffer); // Free the buffer now that we're done with it
    return (0); // Return indicating success
}

