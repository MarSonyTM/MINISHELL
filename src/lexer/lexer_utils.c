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

void	add_token(t_token **tokens, t_token_type type, char *value)
{
	t_token	*new_token;
	t_token	*temp;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->type = type;
	new_token->value = ft_strdup(value);
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
}
