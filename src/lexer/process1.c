#include "../../inc/minishell.h"

void process_whitespace(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env) {
    if (*bufIndex > 0) {
        buffer[*bufIndex] = '\0'; // Null-terminate the current token
        add_token(*tokens, determine_token_type(buffer, 0, env, *TokenCount), strdup(buffer)); // Add the token
        *bufIndex = 0; // Reset buffer index for the next token
        (*TokenCount)++;
    }
}

bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

void process_pipe(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env) {
    buffer[*bufIndex] = '\0'; // Null-terminate the current token if there is any
    if (*bufIndex > 0) {
        add_token(*tokens, determine_token_type(buffer, 0, env, *TokenCount), ft_strdup(buffer));
        *bufIndex = 0; // Reset buffer index for the next token
    }
    add_token(*tokens, TOKEN_PIPE, ft_strdup("|"));
    *TokenCount = 0; // Reset TokenCount after adding a pipe token
}
