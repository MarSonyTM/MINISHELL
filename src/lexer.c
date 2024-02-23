#include "../inc/minishell.h"

void free_tokens(t_token **tokens) {
    t_token *current;
    t_token *temp;

    current = *tokens;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp->value);
        free(temp);
    }
    *tokens = NULL;
}

void add_token(t_token **tokens, t_token_type type, char *value) {
    t_token *new_token;
    t_token *temp;

    new_token = malloc(sizeof(t_token));
    if (!new_token)
        return;
    new_token->type = type;
    new_token->value = strdup(value);
    new_token->next = NULL;
    if (*tokens == NULL)
        *tokens = new_token;
    else {
        temp = *tokens;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_token;
    }
}

void lexer(char *input, t_token **tokens) {
    char *token;
    const char delimiters[] = " \t\n"; // Whitespace characters used as delimiters

    // Tokenize the input string using strtok
    token = strtok(input, delimiters);
    while (token != NULL) {
        if (strcmp(token, "echo") == 0 ||
            strcmp(token, "cd") == 0 ||
            strcmp(token, "pwd") == 0 ||
            strcmp(token, "export") == 0 ||
            strcmp(token, "unset") == 0 ||
            strcmp(token, "env") == 0 ||
            strcmp(token, "exit") == 0) {
            add_token(tokens, TOKEN_COMMAND, strdup(token));
        } else if (strcmp(token, "$?") == 0) {
            add_token(tokens, TOKEN_EXIT_STATUS, strdup(token));
        } else if (token[0] == '$') {
            add_token(tokens, TOKEN_ENV_VAR, strdup(token));
        } else if (strcmp(token, "|") == 0) {
            add_token(tokens, TOKEN_PIPE, strdup(token));
        } else if (strcmp(token, "<") == 0) {
            add_token(tokens, TOKEN_REDIRECT_IN, strdup(token));
        } else if (strcmp(token, ">") == 0) {
            add_token(tokens, TOKEN_REDIRECT_OUT, strdup(token));
        } else if (token[0] == '\'' && token[strlen(token) - 1] == '\'') {
            add_token(tokens, TOKEN_QUOTE, strdup(token));
        } else if (token[0] == '\"' && token[strlen(token) - 1] == '\"') {
            add_token(tokens, TOKEN_DQUOTE, strdup(token));
        } else {
            add_token(tokens, TOKEN_ARG, strdup(token));
        }

        // Get the next token
        token = strtok(NULL, delimiters);
    }
}
