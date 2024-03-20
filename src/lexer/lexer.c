
#include "../../inc/minishell.h"


// Main lexer function
int lexer(char *input, t_token **tokens) {
    int i = 0, bufIndex = 0, inQuote = 0;
    bool quote_error = false;
    char currentChar;
    char *buffer = malloc((ft_strlen(input) + 1) * sizeof(char));
    if (buffer == NULL) {
        // Handle memory allocation error
        return 1;
    }

    while ((currentChar = input[i]) != '\0' && !quote_error) {
        if ((currentChar == ' ' || currentChar == '\t' || currentChar == '\n') && inQuote == 0) {
            if (process_whitespace(&buffer, &bufIndex, tokens, inQuote)) return 1;
        } else if (currentChar == '\'' || currentChar == '\"') {
            handle_quote(currentChar, &inQuote, &buffer, &bufIndex);
        } else if ((currentChar == ',' || currentChar == '<' || currentChar == '>') && inQuote == 0) {
            if (process_special_character(&buffer, &bufIndex, tokens, currentChar, input[i + 1], &i, inQuote)) return 1;
        } else if (currentChar == '$') {
            if (handle_variable(&buffer, &bufIndex, tokens, currentChar, input[i + 1], &i, inQuote)) return 1;
        } else {
            buffer[bufIndex++] = currentChar;
        }
        i++;
    }

    if (finalize_token_addition(&buffer, bufIndex, tokens, inQuote)) return 1;

    free(buffer);
    return quote_error ? 1 : 0;
}

