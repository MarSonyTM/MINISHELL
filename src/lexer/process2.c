#include "../../inc/minishell.h"

int process_single_char_redirection(char currentChar, char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env) {
    if (*bufIndex > 0) {
        buffer[*bufIndex] = '\0';
        if (add_token(*tokens, determine_token_type(buffer, 0, env, TokenCount), ft_strdup(buffer)) == 1)
            return (2);
        *bufIndex = 0;
    }
    t_token_type type = currentChar == '<' ? TOKEN_REDIRECT_IN : TOKEN_REDIRECT_OUT;
    if (add_token(*tokens, type, ft_strdup(&currentChar)) == 1)
        return (2);
    (*TokenCount)++;
    return (0);
}

int process_double_char_redirection(char currentChar, char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, int *i) {
    if (*bufIndex > 0) 
    {
        buffer[*bufIndex] = '\0';
        if (add_token(*tokens, determine_token_type(buffer, 0, env, TokenCount), ft_strdup(buffer)) == 1)
            return (2);
    
        *bufIndex = 0;
    }
    t_token_type type = currentChar == '<' ? TOKEN_HEREDOC : TOKEN_REDIRECT_OUT_APPEND;
    char redirection[3] = {currentChar, currentChar, '\0'};
    if (add_token(*tokens, type, ft_strdup(redirection)) == 1)
        return (2);
    (*TokenCount)++;
    (*i)++; // Skip the next character
    return (0);
}

