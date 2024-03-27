#include "../../inc/minishell.h"

int lexer(char *input, t_token **tokens, t_env *env) 
{
    int i = 0; // Index for input string
    char currentChar; // Current character being processed
    char *buffer = malloc((ft_strlen(input) + 1) * sizeof(char));
    if (buffer == NULL) 
        return (1); // Handle error if malloc fails
    int TokenCount = 0; // Number of tokens
    int bufIndex = 0; // Index for buffer
    int inQuote = 0; // 0: no quote, 1: single quote, 2: double quote
    bool quote_error = false; // Track if there's an unclosed quote error
    while ((currentChar = input[i]) != '\0' && !quote_error) 
    {
        process_input_char(currentChar, input, &buffer, &bufIndex, &tokens, &TokenCount, env, &i, &inQuote, &quote_error);
        i++;
    }
int finalResult = finalize_tokens(&buffer, bufIndex, &tokens, &TokenCount, inQuote, quote_error, env); // Finalize the tokens
free(buffer); // Ensure to free the buffer if it's not needed anymore
return (finalResult);
}
 