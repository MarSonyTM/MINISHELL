#include "../../inc/minishell.h"
 
t_token_type	determine_token_type(char *token, int inQuote, t_env *env, int TokenCount) 
{
	if (TokenCount == 0)
	{
		return (handle_first_token(token));
	}
	return (handle_subsequent_tokens(token, inQuote, env));
}

t_token_type	handle_first_token(char *token) 
{
	const char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};
	int i = 0;
	while (builtins[i]) 
    {
		if (ft_strcmp(token, builtins[i]) == 0) 
		{
            return (TOKEN_BUILTIN);
		}
		i++;
	}
	return (TOKEN_COMMAND);
}

t_token_type handle_subsequent_tokens(char *token, int inQuote, t_env *env) 
{
    t_token_type specialToken = check_special_tokens(token);
    if (specialToken != TOKEN_ARG) 
    {
        return (specialToken);
    }
    if (token[0] == '$') 
    {
        return handle_dollar_tokens(token, inQuote);
    }
    if (is_command(token, env)) 
    {
        return (TOKEN_COMMAND);
    }
    return (TOKEN_ARG);
}

t_token_type check_special_tokens(char *token) 
{
    if (ft_strcmp(token, "|") == 0) return (TOKEN_PIPE);
    if (ft_strcmp(token, "<") == 0) return (TOKEN_REDIRECT_IN);
    if (ft_strcmp(token, ">") == 0) return (TOKEN_REDIRECT_OUT);
    if (ft_strcmp(token, ">>") == 0) return (TOKEN_REDIRECT_OUT_APPEND);
    if (ft_strcmp(token, "<<") == 0) return (TOKEN_HEREDOC);
    if (ft_strcmp(token, ",") == 0) return (TOKEN_COMMA);
    if (ft_strcmp(token, "..") == 0) return (TOKEN_ARG);
    return TOKEN_ARG;
}

t_token_type handle_dollar_tokens(char *token, int inQuote) 
{
    if (inQuote == 2 || inQuote == 1) return (TOKEN_ARG);
    if (strcmp(token, "$?") == 0) return (TOKEN_EXIT_STATUS);
    return (TOKEN_ENV_VAR);
}


