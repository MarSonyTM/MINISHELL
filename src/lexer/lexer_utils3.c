#include "../../inc/minishell.h"

 int handle_redirect_in(char *buffer, int *bufIndex, t_token **tokens, int inQuote) {
    if (*bufIndex > 0) {
        buffer[*bufIndex] = '\0';
        if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1)
            return (1); // Error
        *bufIndex = 0;
    }
    if (add_token(tokens, TOKEN_REDIRECT_IN, ft_strdup("<")) == 1)
        return (1); // Error
    return (0);
}

int handle_redirect_out(char *buffer, int *bufIndex, t_token **tokens, int inQuote) {
    if (*bufIndex > 0) {
        buffer[*bufIndex] = '\0';
        if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1)
            return (1); // Error
        *bufIndex = 0;
    }
    if (add_token(tokens, TOKEN_REDIRECT_OUT, ft_strdup(">")) == 1)
        return (1); // Error
    return (0);
}

int handle_exit_status(char *buffer, int *bufIndex, t_token **tokens, int inQuote, int *i) {
    if (*bufIndex > 0) {
        buffer[*bufIndex] = '\0';
        if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1)
            return (1); // Error
        *bufIndex = 0;
    }
    if (add_token(tokens, TOKEN_EXIT_STATUS, ft_strdup("$?")) == 1)
        return (1); // Error
    (*i)++; // Move past the '?'
    return (0);
}

int handle_heredoc(char *buffer, int *bufIndex, t_token **tokens, int inQuote, int *i) {
    if (*bufIndex > 0) {
        buffer[*bufIndex] = '\0';
        if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1)
            return (1); // Error
        *bufIndex = 0;
    }
    if (add_token(tokens, TOKEN_HEREDOC, ft_strdup("<<")) == 1)
        return (1); // Error
    (*i)++; // Move past the second '<'
    return (0);
}

int handle_redirect_out_append(char *buffer, int *bufIndex, t_token **tokens, int inQuote, int *i) {
    if (*bufIndex > 0) {
        buffer[*bufIndex] = '\0';
        if (add_token(tokens, determine_token_type(buffer, inQuote), ft_strdup(buffer)) == 1)
            return (1); // Error
        *bufIndex = 0;
    }
    if (add_token(tokens, TOKEN_REDIRECT_OUT_APPEND, ft_strdup(">>")) == 1)
        return (1); // Error
    (*i)++; // Move past the second '>'
    return (0);
}