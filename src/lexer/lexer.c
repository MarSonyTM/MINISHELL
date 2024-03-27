#include "../../inc/minishell.h"


int lexer(char *input, t_token **tokens, t_env *env) 
{
    int i = 0; // Index for input string
    char *buffer = malloc((ft_strlen(input) + 1) * sizeof(char));
    int TokenCount = 0; // Number of tokens
    int bufIndex = 0; // Index for buffer
    int inQuote = 0; // 0: no quote, 1: single quote, 2: double quote
    bool quote_error = false; // Track if there's an unclosed quote error
    if (buffer == NULL) // Check for allocation error
        return (1);

    // Process the input string
    process_input_loop(input, &buffer, &bufIndex, &tokens, &TokenCount, env, &i, &inQuote, &quote_error);

    return finalize_lexer(&buffer, bufIndex, &tokens, &TokenCount, inQuote, quote_error, env);
}

