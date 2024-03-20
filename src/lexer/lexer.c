#include "../../inc/minishell.h"

int lexer(char *input, t_token **tokens) 
{
    int i = 0; // Index for input string
    char currentChar; // Current character being processed
    char *buffer = malloc((ft_strlen(input) + 1) * sizeof(char));
    if (buffer == NULL) {
        // Handle memory allocation error
        // printf("Error: Failed to allocate memory for buffer\n");
        return (1);
    }
    int bufIndex = 0; // Index for buffer
    int inQuote = 0; // 0: no quote, 1: single quote, 2: double quote
    bool quote_error = false; // Track if there's an unclosed quote error

    while ((currentChar = input[i]) != '\0' && !quote_error) {
        if ((currentChar == ' ' || currentChar == '\t' || currentChar == '\n') && inQuote == 0) {
            // Whitespace, outside of quotes
            if (bufIndex > 0) {
                handle_whitespace(buffer, &bufIndex, tokens, inQuote);
            }
        } else if (currentChar == '\'' || currentChar == '\"') {
            // Quote handling
            handle_quotes(buffer, &bufIndex, currentChar, &inQuote);
        } else if (currentChar == ',' && inQuote == 0) {
            // Commas outside of quotes are treated as separate tokens
            handle_comma(buffer, &bufIndex, tokens, inQuote);
        } else if (currentChar == '<' && input[i + 1] != '<' && inQuote == 0) {
            // Check if the next character is not '<' and not in a quote
            // This indicates a redirect in token
            handle_redirect_in(buffer, &bufIndex, tokens, inQuote);
        } else if (currentChar == '>' && input[i + 1] != '>' && inQuote == 0) {
            // Check if the next character is not '>' and not in a quote
            // This indicates a redirect out token
            handle_redirect_out(buffer, &bufIndex, tokens, inQuote);
        } else if (currentChar == '$' && input[i + 1] == '?') {
            // Check if the next character is '?' and not in a quote
            // This indicates an exit status token
            handle_exit_status(buffer, &bufIndex, tokens, inQuote, &i);
        } else if (currentChar == '$' && input[i + 1] == '\0') {
            add_token(tokens, TOKEN_ARG, ft_strdup("$"));
        } else if (currentChar == '$' && ft_isalpha(input[i + 1]) == 0) {
            buffer[bufIndex] = '\0';
        } else if (currentChar == '<' && input[i + 1] == '<' && inQuote == 0) {
            // Check if the next character is also '<' and not in a quote
            // This indicates a heredoc token
            handle_heredoc(buffer, &bufIndex, tokens, inQuote, &i);
        } else if (currentChar == '>' && input[i + 1] == '>' && inQuote == 0) {
            // Check if the next character is also '>' and not in a quote
            // This indicates a redirect out append token
            handle_redirect_out_append(buffer, &bufIndex, tokens, inQuote, &i);
        } else {
            // Regular character, add to the buffer
            buffer[bufIndex++] = currentChar;
        }
        i++;
    }

    if (bufIndex > 0 && !quote_error) {
        if (inQuote != 0) {
            error(ERR_QUOT, NULL);
            quote_error = true;
        } else {
            buffer[bufIndex] = '\0';
            if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1)
                return (1); // Error
        }
    } else if (inQuote != 0 && !quote_error) {
        error(ERR_QUOT, NULL);
        quote_error = true;
    }

    if (quote_error) {
        free(buffer);
        return (1);
    }

    return (0);
}

