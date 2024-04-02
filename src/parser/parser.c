#include "../../inc/minishell.h"

// Main parse function
int parse(t_token *tokens, t_cmd **cmd, t_env *env)
{
	processTokens(tokens, cmd, env);
    return (0);
} 
