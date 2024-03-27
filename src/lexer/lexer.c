#include "../../inc/minishell.h"

int lexer(char *input, t_token **tokens, t_env *env) 
{
    int i = 0; // Index for input string
    char currentChar; // Current character being processed
    char *buffer = malloc((ft_strlen(input) + 1) * sizeof(char));
    int TokenCount = 0; // Number of tokens

    if (buffer == NULL) // Check for allocation error
        return (1);
    int bufIndex = 0; // Index for buffer
    int inQuote = 0; // 0: no quote, 1: single quote, 2: double quote
    bool quote_error = false; // Track if there's an unclosed quote error

    while ((currentChar = input[i]) != '\0' && !quote_error) {
        if (is_whitespace(currentChar) && inQuote == 0) { 
            // Call the helper function instead of inline code
            process_whitespace(buffer, &bufIndex, &tokens, &TokenCount, env);
        }

        else if (currentChar == '|')
         {
            // Call the helper function instead of inline code
            process_pipe(buffer, &bufIndex, &tokens, &TokenCount, env);
}
        else if (currentChar == '\'' || currentChar == '\"') 
        {
            // Call the helper function instead of inline code
            process_quotes(currentChar, &buffer, &bufIndex, &inQuote);
        } 
        else if (currentChar == ',' && inQuote == 0) 
        {
             process_comma(buffer, &bufIndex, &tokens, &TokenCount, env);
        } 
        else if (currentChar == '<' && input[i + 1] != '<' && inQuote == 0)
         {
             process_single_char_redirection(currentChar, buffer, &bufIndex, &tokens, &TokenCount, env);
        } 
        else if (currentChar == '>' && input[i + 1] != '>' && inQuote == 0)
        {
            process_double_char_redirection(currentChar, buffer, &bufIndex, &tokens, &TokenCount, env, &i);
        }
        else if (currentChar == '$')
        {
            process_dollar_conditions(input, &i, &buffer, &bufIndex, &tokens, &TokenCount, env, inQuote);
        }
        else if (currentChar == '<' && input[i + 1] == '<' && inQuote == 0)
        {
            process_heredoc(&buffer, &bufIndex, &tokens, &TokenCount, env, &i);
        } 
        else if (currentChar == '>' && input[i + 1] == '>' && inQuote == 0)
        {
            // Check if the next character is also '>' and not in a quote
            // This indicates a redirect out append token
            if (bufIndex > 0)
         {
                buffer[bufIndex] = '\0'; // Null-terminate the current token
                if (add_token(tokens, determine_token_type(buffer, inQuote, env, &TokenCount), ft_strdup(buffer)) == 1) // Add the token
                    return (1); // Error
                bufIndex = 0; // Reset buffer index for the next token
                TokenCount++;
            }
            // Add the redirect out append token
            if (add_token(tokens, TOKEN_REDIRECT_OUT_APPEND, ft_strdup(">>")) == 1) // Add the token
                return (1); // Error
            TokenCount++;
            i++; // Move past the second '>'
        }
        else
            // Regular character, add to the buffer
            buffer[bufIndex++] = currentChar;
        i++;
    }
    // Check if there's a token at the end of the input
    if (bufIndex > 0 && !quote_error) 
    {
        if (inQuote != 0) 
        { // If we ended in a quote, it's an unclosed quote error
            error(ERR_QUOT, NULL);
            quote_error = true; // Set error to prevent adding the token
        } 
        else 
        {
            buffer[bufIndex] = '\0'; // Null-terminate the current token

            if (add_token(tokens, determine_token_type(buffer, inQuote, env, &TokenCount), ft_strdup(buffer)) == 1) // Add the token
                return (2); // Error
            TokenCount++;
        }
    } 
    else if (inQuote != 0 && !quote_error) 
    {
        // Handle the case where the input ends while still in a quote
        error(ERR_QUOT, NULL);
        quote_error = true; // Set error to prevent adding the token
    }
    if (quote_error) 
    {
        // Free the tokens if there was an error
        //  free_tokens(tokens); //segfaults here!!!!
         free(buffer);
        return (2); // Return without processing the tokens
    }
    return (0);
}
