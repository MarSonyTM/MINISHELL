#include "../inc/minishell.h"

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
	new_token->value = strdup(value);
	new_token->next = NULL;
	if (*tokens == NULL)
		*tokens = new_token;
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

void lexer(char *input, t_token **tokens)
{
    char *token_start = NULL;
    int i = 0;
    int in_single_quote = 0;
    int in_double_quote = 0;

    while (input[i] != '\0')
    {
        if (input[i] == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            if (!in_single_quote && token_start != NULL)
            {
                add_token(tokens, TOKEN_QUOTE, strndup(token_start, &input[i] - token_start));
                token_start = NULL;
            }
            else if (in_single_quote)
            {
                token_start = &input[i + 1];
            }
        }
        else if (input[i] == '\"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            if (!in_double_quote && token_start != NULL)
            {
                add_token(tokens, TOKEN_DQUOTE, strndup(token_start, &input[i] - token_start));
                token_start = NULL;
            }
            else if (in_double_quote)
            {
                token_start = &input[i + 1];
            }
        }
        else if (!in_single_quote && !in_double_quote)
        {
            if (input[i] == ' ')
            {
                if (token_start != NULL)
                {
                    if (strcmp(token_start, "echo") == 0 ||
                        strcmp(token_start, "cd") == 0 ||
                        strcmp(token_start, "pwd") == 0 ||
                        strcmp(token_start, "export") == 0 ||
                        strcmp(token_start, "unset") == 0 ||
                        strcmp(token_start, "env") == 0 ||
                        strcmp(token_start, "exit") == 0)
                    {
                        add_token(tokens, TOKEN_COMMAND, strdup(token_start));
                        token_start = NULL;
                    }
                    else
                    {
                        add_token(tokens, TOKEN_ARG, strndup(token_start, &input[i] - token_start));
                        token_start = NULL;
                    }
                }
            }
            else if (input[i] == '|')
            {
                if (token_start != NULL)
                {
                    add_token(tokens, TOKEN_ARG, strndup(token_start, &input[i] - token_start));
                    token_start = NULL;
                }
                add_token(tokens, TOKEN_PIPE, strdup("|"));
            }
            else if (input[i] == '<')
            {
                if (token_start != NULL)
                {
                    add_token(tokens, TOKEN_ARG, strndup(token_start, &input[i] - token_start));
                    token_start = NULL;
                }
                add_token(tokens, TOKEN_REDIRECT_IN, strdup("<"));
            }
            else if (input[i] == '>')
            {
                if (token_start != NULL)
                {
                    add_token(tokens, TOKEN_ARG, strndup(token_start, &input[i] - token_start));
                    token_start = NULL;
                }
                if (input[i + 1] == '>')
                {
                    add_token(tokens, TOKEN_DOUBLE_REDIRECT_OUT, strdup(">>"));
                    i++; // Skip the next character as it's part of the redirection operator
                }
                else
                {
                    add_token(tokens, TOKEN_REDIRECT_OUT, strdup(">"));
                }
            }
            else if (input[i] == '$')
            {
                if (isalpha(input[i + 1]) || input[i + 1] == '?')
                {
                    // Environment variable or special variable $?
                    if (input[i + 1] == '?')
                    {
                        add_token(tokens, TOKEN_EXIT_STATUS, strdup("$?"));
                        i++; // Skip the next character as it's part of the special variable
                    }
                    else
                    {
                        add_token(tokens, TOKEN_ENV_VAR, strdup("$"));
                    }
                }
                else
                {
                    // Just a dollar sign
                    add_token(tokens, TOKEN_ARG, strdup("$"));
                }
            }
            else if (input[i] != '\0' && token_start == NULL)
            {
                token_start = &input[i];
            }
        }

        i++;
    }

    if (token_start != NULL)
    {
        if (strcmp(token_start, "echo") == 0 ||
            strcmp(token_start, "cd") == 0 ||
            strcmp(token_start, "pwd") == 0 ||
            strcmp(token_start, "export") == 0 ||
            strcmp(token_start, "unset") == 0 ||
            strcmp(token_start, "env") == 0 ||
            strcmp(token_start, "exit") == 0)
        {
            add_token(tokens, TOKEN_COMMAND, strdup(token_start));
        }
        else if (strcmp(token_start, "$?") == 0)
        {
            add_token(tokens, TOKEN_EXIT_STATUS, strdup(token_start));
        }
        else if (token_start[0] == '$')
        {
            add_token(tokens, TOKEN_ENV_VAR, strdup(token_start));
        }
        else
        {
            add_token(tokens, TOKEN_ARG, strdup(token_start));
        }
    }
}
printf