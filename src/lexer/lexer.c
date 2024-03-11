#include "../../inc/minishell.h"

void lexer(char *input, t_token **tokens) // takes in a string and a pointer to a pointer to a token
{
    int i = 0; // Index for input string
    char currentChar; // Current character being processed
    char *buffer = malloc((strlen(input) + 1) * sizeof(char));
    if (buffer == NULL) 
    {
        // Handle memory allocation error
        printf("Error: Failed to allocate memory for buffer\n");
        return ;
    }
    int bufIndex = 0; // Index for buffer
    int inQuote = 0; // 0: no quote, 1: single quote, 2: double quote
    bool quote_error = false; // Track if there's an unclosed quote error

    while ((currentChar = input[i]) != '\0' && !quote_error)
    {
        if ((currentChar == ' ' || currentChar == '\t' || currentChar == '\n') && inQuote == 0) // Whitespace , outside of quotes
        {
            // End of a token
            if (bufIndex > 0) // If there's a token in the buffer
            {
                buffer[bufIndex] = '\0'; // Null-terminate the current token
                add_token(tokens, determine_token_type(buffer), strdup(buffer)); // Add the token
                bufIndex = 0; // Reset buffer index for the next token
            }
        } 
        else if (currentChar == '\'' || currentChar == '\"') // Quote handling
        {
            if (inQuote == 0) { // Starting a quote
                inQuote = (currentChar == '\'') ? 1 : 2;
            } 
            else if ((inQuote == 1 && currentChar == '\'') || (inQuote == 2 && currentChar == '\"'))
            {
                // Ending a quote
                inQuote = 0;
            } 
            else 
            {
                // Inside quotes, treat as part of the token
                buffer[bufIndex++] = currentChar;
            }
        } 
        else if (currentChar == ',' && inQuote == 0) 
        {
            // Commas outside of quotes are treated as separate tokens
            if (bufIndex > 0) 
            {
                // Add the current token before the comma
                buffer[bufIndex] = '\0';
                add_token(tokens, determine_token_type(buffer), strdup(buffer));
                bufIndex = 0;
            }
            // Add the comma as a separate token
            add_token(tokens, TOKEN_COMMA, strdup(","));
        } 
        else 
        {
            // Regular character, add to the buffer
            buffer[bufIndex++] = currentChar;
        }
        i++;
    }
    // Check if there's a token at the end of the input
    if (bufIndex > 0 && !quote_error) 
    {
        if (inQuote != 0) 
        { // If we ended in a quote, it's an unclosed quote error
            printf("Error: Unclosed quote\n");
            quote_error = true; // Set error to prevent adding the token
        } 
        else 
        {
            buffer[bufIndex] = '\0'; // Null-terminate the current token
            add_token(tokens, determine_token_type(buffer), strdup(buffer)); // Add the token
        }
    }
    else if (inQuote != 0 && !quote_error) 
    {
        // Handle the case where the input ends while still in a quote
        printf("Error: Unclosed quote\n");
         // Free the buffer  
        quote_error = true; // Set error to prevent adding the token
    }
    if (quote_error) 
    {
        // Free the tokens if there was an error
        free_tokens(tokens);
        free(buffer);
    }
}
// Placeholder for determine_token_type function
t_token_type determine_token_type(char *token)
{
    // Check for specific commands or symbols
    if (ft_strcmp(token, "|") == 0) return TOKEN_PIPE;
    else if (ft_strcmp(token, "<") == 0) return TOKEN_REDIRECT_IN;
    else if (ft_strcmp(token, ">") == 0) return TOKEN_REDIRECT_OUT;
    else if (ft_strcmp(token, ">>") == 0) return TOKEN_REDIRECT_OUT_APPEND;
    else if (ft_strcmp(token, "<<") == 0) return TOKEN_HEREDOC;
    else if (ft_strcmp(token, ",") == 0) return TOKEN_COMMA;
    else if (token[0] == '$') 
    {
        // Check for exit status specifically, or default to env var
        if (ft_strcmp(token, "$?") == 0) return TOKEN_EXIT_STATUS;
        else return TOKEN_ENV_VAR;
    }
    else if (token[0] == '\"') return TOKEN_DQUOTE;
    else if (token[0] == '\'') return TOKEN_QUOTE;
    // Check for specific commands
    else if (ft_strcmp(token, "echo") == 0 || ft_strcmp(token, "cd") == 0 ||
             ft_strcmp(token, "pwd") == 0 || ft_strcmp(token, "export") == 0 ||
             ft_strcmp(token, "unset") == 0 || ft_strcmp(token, "env") == 0 ||
             ft_strcmp(token, "exit") == 0) 
    {
        return TOKEN_BUILTIN;
    }
     char *path = getenv("PATH");
    char *pathCopy = ft_strdup(path);
    char *dir = ft_strtok(pathCopy, ":");

    while (dir != NULL) {
        char *fullPath = malloc(ft_strlen(dir) + ft_strlen(token) + 2); // For '/' and '\0'
        sprintf(fullPath, "%s/%s", dir, token);
        if (access(fullPath, X_OK) == 0) {
            free(pathCopy);
            free(fullPath);
            return TOKEN_COMMAND; // Token corresponds to an executable file
        }
        free(fullPath);
        dir = ft_strtok(NULL, ":");
    }

    free(pathCopy);
    return TOKEN_ARG; // Token is not an executable file
}

