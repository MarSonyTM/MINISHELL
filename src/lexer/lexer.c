#include "../../inc/minishell.h"

void lexer(char *input, t_token **tokens) 
{
    int i = 0; // Index for input string
    char currentChar; // Current character being processed
    char *buffer = malloc((ft_strlen(input) + 1) * sizeof(char));
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
        if ((currentChar == ' ' || currentChar == '\t' || currentChar == '\n') && inQuote == 0) 
        {
            // Whitespace, outside of quotes
            if (bufIndex > 0)
             {
                buffer[bufIndex] = '\0'; // Null-terminate the current token
                add_token(tokens, determine_token_type(buffer), ft_strdup(buffer)); // Add the token
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
                add_token(tokens, determine_token_type(buffer), ft_strdup(buffer));
                bufIndex = 0;
            }
            // Add the comma as a separate token
            add_token(tokens, TOKEN_COMMA, ft_strdup(","));
        } 
        else if (currentChar == '<' && input[i + 1] != '<' && inQuote == 0)
         {
            // Check if the next character is not '<' and not in a quote
            // This indicates a redirect in token
            if (bufIndex > 0) 
            {
                buffer[bufIndex] = '\0'; // Null-terminate the current token
                add_token(tokens, determine_token_type(buffer), ft_strdup(buffer)); // Add the token
                bufIndex = 0; // Reset buffer index for the next token
            }
            // Add the redirect in token
            add_token(tokens, TOKEN_REDIRECT_IN, ft_strdup("<"));
        } 
        else if (currentChar == '>' && input[i + 1] != '>' && inQuote == 0)
        {
            // Check if the next character is not '>' and not in a quote
            // This indicates a redirect out token
            if (bufIndex > 0)
            {
                buffer[bufIndex] = '\0'; // Null-terminate the current token
                add_token(tokens, determine_token_type(buffer), ft_strdup(buffer)); // Add the token
                bufIndex = 0; // Reset buffer index for the next token
            }
            // Add the redirect out token
            add_token(tokens, TOKEN_REDIRECT_OUT, ft_strdup(">"));
        } 
        else if (currentChar == '<' && input[i + 1] == '<' && inQuote == 0)
        {
            // Check if the next character is also '<' and not in a quote
            // This indicates a heredoc token
            if (bufIndex > 0) 
            {
                buffer[bufIndex] = '\0'; // Null-terminate the current token
                add_token(tokens, determine_token_type(buffer), ft_strdup(buffer)); // Add the token
                bufIndex = 0; // Reset buffer index for the next token
            }
            // Add the heredoc token
            add_token(tokens, TOKEN_HEREDOC, ft_strdup("<<"));
            i++; // Move past the second '<'
        } 
        else if (currentChar == '>' && input[i + 1] == '>' && inQuote == 0)
        {
            // Check if the next character is also '>' and not in a quote
            // This indicates a redirect out append token
            if (bufIndex > 0)
         {
                buffer[bufIndex] = '\0'; // Null-terminate the current token
                add_token(tokens, determine_token_type(buffer), ft_strdup(buffer)); // Add the token
                bufIndex = 0; // Reset buffer index for the next token
            }
            // Add the redirect out append token
            add_token(tokens, TOKEN_REDIRECT_OUT_APPEND, ft_strdup(">>"));
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
            printf("Error: Unclosed quote\n");
            quote_error = true; // Set error to prevent adding the token
        } 
        else 
        {
            buffer[bufIndex] = '\0'; // Null-terminate the current token
            add_token(tokens, determine_token_type(buffer), ft_strdup(buffer)); // Add the token
        }
    } 
    else if (inQuote != 0 && !quote_error) 
    {
        // Handle the case where the input ends while still in a quote
        printf("Error: Unclosed quote\n");
        quote_error = true; // Set error to prevent adding the token
    }
    if (quote_error) 
    {
        // Free the tokens if there was an error
        //  free_tokens(tokens); //segfaults here!!!!
         free(buffer);
        return ; // Return without processing the tokens
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
        if (token[0] == '$') 
{
    // Directly check the next character in the token for a quote
    if (token[1] == '\"' || token[1] == '\'') 
    {
        // This means we have something like $"..." or $'...', treat as ARG
        return TOKEN_ARG;
    } 
    else 
    {
        // Handling other $ cases, such as $USER or $? which are ENV_VAR or special
        if (strcmp(token, "$?") == 0) return TOKEN_EXIT_STATUS;
        else return TOKEN_ENV_VAR;
    }
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

while (dir != NULL)
{
    // Calculate the length of the directory path, token, '/' and null terminator
    size_t fullPathLen = ft_strlen(dir) + ft_strlen(token) + 2;
    char *fullPath = malloc(fullPathLen);

    // Check if malloc succeeded
    if (fullPath == NULL)
    {
        printf("Error: Failed to allocate memory for fullPath\n");
        free(pathCopy);
        return TOKEN_ERROR; // Indicate an error condition
    }

    // Copy the directory path into fullPath
    ft_strcpy(fullPath, dir);

    // Append '/' if the directory path doesn't end with '/'
    if (fullPath[ft_strlen(fullPath) - 1] != '/')
        ft_strcat(fullPath, "/");
    
    // Append the token to fullPath
    ft_strcat(fullPath, token);

    // Check if the concatenated path exists and is executable
    if (access(fullPath, X_OK) == 0) 
    {
        free(pathCopy);
        free(fullPath);
        return TOKEN_COMMAND; // Token corresponds to an executable file
    }

    // Free the memory allocated for fullPath
    free(fullPath);

    // Move to the next directory in PATH
    dir = ft_strtok(NULL, ":");
}

// Free the memory allocated for pathCopy
free(pathCopy);

return TOKEN_ARG; // Token is not an executable file
}

