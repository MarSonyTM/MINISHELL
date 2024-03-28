#include "../../inc/minishell.h"

int	process_whitespace(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env) 
{
    if (*bufIndex > 0) 
    {
        buffer[*bufIndex] = '\0'; // Null-terminate the current token
        if (add_token(*tokens, determine_token_type(buffer, 0, env, TokenCount), strdup(buffer)) == 1) // Add the token
            return (1);
        *bufIndex = 0; // Reset buffer index for the next token
        TokenCount++;
    }
    return (0);
}

bool is_whitespace(char c) 
{
    return (c == ' ' || c == '\t' || c == '\n');
}

int	process_pipe(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env)
{
    buffer[*bufIndex] = '\0'; // Null-terminate the current token if there is any
    if (*bufIndex > 0) 
	{
        if (add_token(*tokens, determine_token_type(buffer, 0, env, TokenCount), ft_strdup(buffer)) == 1) 
			return (1); // Handle error appropriately
        *bufIndex = 0; // Reset buffer index for the next token
    }
    if (add_token(*tokens, TOKEN_PIPE, ft_strdup("|")) == 1)
		return (1); // Handle error appropriately
    *TokenCount = 0; // Reset TokenCount after adding a pipe token
    return (0);
}

void	process_quotes(char currentChar, char **buffer, int *bufIndex, int *inQuote) 
{
	if (*inQuote == 0) 
	{
        // Starting a quote
        *inQuote = (currentChar == '\'') ? 1 : 2;
    } 
	else if ((*inQuote == 1 && currentChar == '\'') || (*inQuote == 2 && currentChar == '\"')) 
	{
        // Ending a quote
        *inQuote = 0;
    }
	else 
	{
        // Inside quotes, treat as part of the token
        (*buffer)[(*bufIndex)++] = currentChar;
    }
}

int	process_comma(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env)
{
    if (*bufIndex > 0)
	{
        buffer[*bufIndex] = '\0'; // Null-terminate the current token
        if (add_token(*tokens, determine_token_type(buffer, 0, env, TokenCount), ft_strdup(buffer)) == 1)
			return (1); // Handle error appropriately
        *bufIndex = 0; // Reset buffer index for the next token
        (*TokenCount)++;
    }
    if (add_token(*tokens, TOKEN_COMMA, ft_strdup(",")) == 1)
		return (1); // Handle error appropriately
    (*TokenCount)++;
    return (0);
}
 