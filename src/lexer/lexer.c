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

    while ((currentChar = input[i]) != '\0' && !quote_error)
     {
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
            process_single_redirect_in(buffer, &bufIndex, &tokens, &TokenCount, env, inQuote);
        } 
        else if (currentChar == '>' && input[i + 1] != '>' && inQuote == 0)
        { 
            process_single_redirect_out(buffer, &bufIndex, &tokens, &TokenCount, env);
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
            process_redirect_out_append(buffer, &bufIndex, &tokens, &TokenCount, env, &i, inQuote);
        }
        else
            // Regular character, add to the buffer
            buffer[bufIndex++] = currentChar;
        i++;
    }
     return finalize_lexer(&buffer, bufIndex, &tokens, &TokenCount, inQuote, quote_error, env);
}

