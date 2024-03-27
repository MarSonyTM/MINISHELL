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

int finalize_tokens(char **buffer, int bufIndex, t_token ***tokens, int *TokenCount, int inQuote, bool quote_error, t_env *env) {
    if (bufIndex > 0 && !quote_error) {
        if (inQuote != 0) {
            error(ERR_QUOT, NULL);
            quote_error = true;
        } else {
            (*buffer)[bufIndex] = '\0';
            if (add_token(*tokens, determine_token_type(*buffer, inQuote, env, TokenCount), ft_strdup(*buffer)) == 1) {
                return 2;
            }
            (*TokenCount)++;
        }
    } 
    if (inQuote != 0 && !quote_error) {
        error(ERR_QUOT, NULL);
        quote_error = true;
    }
    if (quote_error) {
        // Assuming free_tokens is a function to free all allocated tokens
        // free_tokens(tokens);
        free(*buffer);
        return 2;
    }
    return 0;
}
