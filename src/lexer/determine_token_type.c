#include "../../inc/minishell.h"

t_token_type	determine_token_type(char *token, int inQuote, t_env *env, int *TokenCount) 
{
	if (*TokenCount == 0)
		return (handle_first_token(token, TokenCount));
	return (handle_subsequent_tokens(token, inQuote, env, TokenCount));
}

t_token_type	handle_first_token(char *token, int *TokenCount) 
{
	const char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};
	int i = 0;
	while (builtins[i]) 
	{
		if (ft_strcmp(token, builtins[i]) == 0) 
		{
			(*TokenCount)++;
			return (TOKEN_BUILTIN);
		}
		i++;
	}
	(*TokenCount)++;
	return (TOKEN_COMMAND);
}

t_token_type	handle_subsequent_tokens(char *token, int inQuote, t_env *env, int *TokenCount)
{
	t_token_type specialToken = check_special_tokens(token);
	if (specialToken != TOKEN_ARG)
		return (specialToken);

	// Assuming you have a reason to check the command only if it's the first token
    // which seems counter-intuitive given the function's purpose.
	if (*TokenCount == 0 && is_command(token, env))
        return (TOKEN_COMMAND);
    	if (token[0] == '$' && token[1] != '\0')
        	return (handle_dollar_tokens(token, inQuote));
	
	return (TOKEN_ARG);
}

t_token_type	check_special_tokens(char *token) 
{
	if (ft_strcmp(token, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(token, "<") == 0)
		return (TOKEN_REDIRECT_IN);
	if (ft_strcmp(token, ">") == 0)
		return (TOKEN_REDIRECT_OUT);
	if (ft_strcmp(token, ">>") == 0)
		return (TOKEN_REDIRECT_OUT_APPEND);
	if (ft_strcmp(token, "<<") == 0)
		return (TOKEN_HEREDOC);
	if (ft_strcmp(token, ",") == 0)
		return (TOKEN_COMMA);
	if (ft_strcmp(token, "..") == 0) 
		return (TOKEN_ARG);
	return (TOKEN_ARG);
}

t_token_type	handle_dollar_tokens(char *token, int inQuote) 
{
	if (inQuote == 2 || inQuote == 1) 
		return (TOKEN_ARG);
	if (strcmp(token, "$?") == 0) 
		return (TOKEN_EXIT_STATUS);
	return (TOKEN_ENV_VAR);
}
