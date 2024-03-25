#include "../../inc/minishell.h"

void process_single_char_redirection(char currentChar, char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env) {
    if (*bufIndex > 0) {
        buffer[*bufIndex] = '\0';
        add_token(*tokens, determine_token_type(buffer, 0, env, TokenCount), ft_strdup(buffer));
        *bufIndex = 0;
    }
    t_token_type type = currentChar == '<' ? TOKEN_REDIRECT_IN : TOKEN_REDIRECT_OUT;
    add_token(*tokens, type, ft_strdup(&currentChar));
    (*TokenCount)++;
}

void process_double_char_redirection(char currentChar, char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, int *i) {
    if (*bufIndex > 0) {
        buffer[*bufIndex] = '\0';
        add_token(*tokens, determine_token_type(buffer, 0, env, TokenCount), ft_strdup(buffer));
        *bufIndex = 0;
    }
    t_token_type type = currentChar == '<' ? TOKEN_HEREDOC : TOKEN_REDIRECT_OUT_APPEND;
    char redirection[3] = {currentChar, currentChar, '\0'};
    add_token(*tokens, type, ft_strdup(redirection));
    (*TokenCount)++;
    (*i)++; // Skip the next character
}

