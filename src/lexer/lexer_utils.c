#include "../../inc/minishell.h"

void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*temp;

	current = *tokens;
	while (current != NULL) 
	{
		temp = current;
		current = current->next;
		free(temp->value);
		free(temp);
	}
	*tokens = NULL;
}

int	add_token(t_token **tokens, t_token_type type, char *value)
{
	t_token	*new_token;
	t_token	*temp;

	if (!value)
		return (1);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (1);
	new_token->type = type;
	new_token->value = ft_strdup(value);
	if (!new_token->value)
		return (1);
	new_token->next = NULL;
	if (*tokens == NULL)
	{
		*tokens = new_token;
	}
	else
	{
		temp = *tokens;
		while (temp->next != NULL) 
		{
			temp = temp->next;
		}
		temp->next = new_token;
	}
	return (0);
}

int handle_whitespace(char *buffer, int *bufIndex, t_token **tokens, int inQuote) {
    buffer[*bufIndex] = '\0'; // Null-terminate the current token
    if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1)
        return (1); // Add the token
    *bufIndex = 0; // Reset buffer index for the next token
    return (0);
}

int handle_quotes(char *buffer, int *bufIndex, char currentChar, int *inQuote) {
    if (*inQuote == 0) { // Starting a quote
        if (currentChar == '\'')
            *inQuote = 1;
        else
            *inQuote = 2;
    } else if ((*inQuote == 1 && currentChar == '\'') || (*inQuote == 2 && currentChar == '\"'))
        // Ending a quote
        *inQuote = 0;
    else
        // Inside quotes, treat as part of the token
        buffer[(*bufIndex)++] = currentChar;
    return (0);
}

int handle_comma(char *buffer, int *bufIndex, t_token **tokens, int inQuote) {
    if (*bufIndex > 0) {
        buffer[*bufIndex] = '\0';
        if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1)
            return (1); // Error
        *bufIndex = 0;
    }
    if (add_token(tokens, TOKEN_COMMA, ft_strdup(",")) == 1)
        return (1); // Error
    return (0);
}

