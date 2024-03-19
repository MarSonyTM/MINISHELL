#include "../../inc/minishell.h"

t_token_type determine_token_type(char *token, int inQuote) 
{
    t_token_type tokenType;

    // Check for specific symbols or commands
    tokenType = check_for_specific_symbols(token);
    if (tokenType != TOKEN_ARG) return tokenType;

    // Handle variable tokens
    tokenType = handle_variable_tokens(token, inQuote);
    if (tokenType != TOKEN_ARG) return tokenType;

    // Check for quotes
    tokenType = check_for_quotes(token[0]);
    if (tokenType != TOKEN_ARG) return tokenType;

    // Check for built-in commands
    tokenType = check_for_built_in_commands(token);
    if (tokenType != TOKEN_ARG) return tokenType;

    // Finally, search for commands in the PATH
    return search_for_commands_in_path(token);
}


t_token_type check_for_specific_symbols(char *token) {
    if (strcmp(token, "|") == 0) return TOKEN_PIPE;
    else if (strcmp(token, "<") == 0) return TOKEN_REDIRECT_IN;
    else if (strcmp(token, ">") == 0) return TOKEN_REDIRECT_OUT;
    else if (strcmp(token, ">>") == 0) return TOKEN_REDIRECT_OUT_APPEND;
    else if (strcmp(token, "<<") == 0) return TOKEN_HEREDOC;
    else if (strcmp(token, ",") == 0) return TOKEN_COMMA;
    else return TOKEN_ARG; // Default case if no specific symbol matches
}

t_token_type handle_variable_tokens(char *token, int inQuote) {
    if (token[0] != '$') return TOKEN_ARG;
    if (inQuote) return TOKEN_ARG; // Treat as argument if within quotes
    if (strcmp(token, "$?") == 0) return TOKEN_EXIT_STATUS;
    return TOKEN_ENV_VAR;
}

t_token_type check_for_quotes(char firstChar) {
    if (firstChar == '\"') return TOKEN_DQUOTE;
    else if (firstChar == '\'') return TOKEN_QUOTE;
    else return TOKEN_ARG;
}

t_token_type check_for_built_in_commands(char *token) {
    if (strcmp(token, "echo") == 0 || strcmp(token, "cd") == 0 ||
        strcmp(token, "pwd") == 0 || strcmp(token, "export") == 0 ||
        strcmp(token, "unset") == 0 || strcmp(token, "env") == 0 ||
        strcmp(token, "exit") == 0) {
        return TOKEN_BUILTIN;
    }
    return TOKEN_ARG;
}