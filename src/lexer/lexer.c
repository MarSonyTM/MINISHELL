#include "../../inc/minishell.h"

int lexer(char *input, t_token **tokens) 
{
    int i = 0; // Index for input string
    char currentChar; // Current character being processed
    char *buffer = malloc((ft_strlen(input) + 1) * sizeof(char));
    if (buffer == NULL) 
    {
        // Handle memory allocation error
        // printf("Error: Failed to allocate memory for buffer\n");
        return (1);
    }
    int bufIndex = 0; // Index for buffer
    int inQuote = 0; // 0: no quote, 1: single quote, 2: double quote
    bool quote_error = false; // Track if there's an unclosed quote error

    while ((currentChar = input[i]) != '\0' && !quote_error)
    {
        if ((currentChar == ' ' || currentChar == '\t' || currentChar == '\n') && inQuote == 0) 
        {
            // Whitespace, outside of quotes
            if (bufIndex > 0)
             {
                buffer[bufIndex] = '\0'; // Null-terminate the current token
                if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1)
                    return (1); // Add the token
                bufIndex = 0; // Reset buffer index for the next token
            }
        } 
        else if (currentChar == '\'' || currentChar == '\"') 
        {
            // Quote handling
            if (inQuote == 0) 
            { // Starting a quote
                if (currentChar == '\'') 
                    inQuote = 1;
                else 
                    inQuote = 2;
                
            } 
            else if ((inQuote == 1 && currentChar == '\'') || (inQuote == 2 && currentChar == '\"'))
                // Ending a quote
                inQuote = 0;
            else 
                // Inside quotes, treat as part of the token
                buffer[bufIndex++] = currentChar;
        } 
        else if (currentChar == ',' && inQuote == 0) 
        {
            // Commas outside of quotes are treated as separate tokens
            if (bufIndex > 0) 
            {
                // Add the current token before the comma
                buffer[bufIndex] = '\0';
                if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1) // Add the token
                    return (1); // Error
                bufIndex = 0;
            }
            // Add the comma as a separate token
            if (add_token(tokens, TOKEN_COMMA, ft_strdup(",")) == 1) // Add the token
                return (1); // Error
        } 
        else if (currentChar == '<' && input[i + 1] != '<' && inQuote == 0)
         {
            // Check if the next character is not '<' and not in a quote
            // This indicates a redirect in token
            if (bufIndex > 0) 
            {
                buffer[bufIndex] = '\0'; // Null-terminate the current token
                if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1) // Add the token
                    return (1); // Error
                bufIndex = 0; // Reset buffer index for the next token
            }
            // Add the redirect in token
            if (add_token(tokens, TOKEN_REDIRECT_IN, ft_strdup("<")) == 1) // Add the token
                return (1); // Error
        } 
        else if (currentChar == '>' && input[i + 1] != '>' && inQuote == 0)
        {
            // Check if the next character is not '>' and not in a quote
            // This indicates a redirect out token
            if (bufIndex > 0)
            {
                buffer[bufIndex] = '\0'; // Null-terminate the current token
                if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1) // Add the token
                    return (1); // Error
                bufIndex = 0; // Reset buffer index for the next token
            }
            // Add the redirect out token
            if (add_token(tokens, TOKEN_REDIRECT_OUT, ft_strdup(">")) == 1) // Add the token
                return (1); // Error
        }
        else if (currentChar == '$' && input[i + 1] == '?')
        {
            // Check if the next character is '?' and not in a quote
            // This indicates an exit status token
            if (bufIndex > 0) 
            {
                buffer[bufIndex] = '\0'; // Null-terminate the current token
                if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1) // Add the token
                    return (1); // Error
                bufIndex = 0; // Reset buffer index for the next token
            }
            // Add the exit status token
            if (add_token(tokens, TOKEN_EXIT_STATUS, ft_strdup("$?")) == 1) // Add the token
                return (1); // Error
            i++; // Move past the '?'
        }
        else if (currentChar == '$' && input[i + 1] == '\0')
        {
            add_token(tokens, TOKEN_ARG, ft_strdup("$"));
        }
        else if (currentChar == '$' && ft_isalpha(input[i + 1]) == 0)
        {
            buffer[bufIndex] = '\0';
        }
        else if (currentChar == '<' && input[i + 1] == '<' && inQuote == 0)
        {
            // Check if the next character is also '<' and not in a quote
            // This indicates a heredoc token
            if (bufIndex > 0) 
            {
                buffer[bufIndex] = '\0'; // Null-terminate the current token
                if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1) // Add the token
                    return (1); // Error
                bufIndex = 0; // Reset buffer index for the next token
            }
            // Add the heredoc token
            if (add_token(tokens, TOKEN_HEREDOC, ft_strdup("<<")) == 1) // Add the token
                return (1); // Error
            i++; // Move past the second '<'
        } 
        else if (currentChar == '>' && input[i + 1] == '>' && inQuote == 0)
        {
            // Check if the next character is also '>' and not in a quote
            // This indicates a redirect out append token
            if (bufIndex > 0)
         {
                buffer[bufIndex] = '\0'; // Null-terminate the current token
                if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1) // Add the token
                    return (1); // Error
                bufIndex = 0; // Reset buffer index for the next token
            }
            // Add the redirect out append token
            if (add_token(tokens, TOKEN_REDIRECT_OUT_APPEND, ft_strdup(">>")) == 1) // Add the token
                return (1); // Error
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
            if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1) // Add the token
                return (1); // Error
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
        return (1); // Return without processing the tokens
    }
    return (0);
}

 