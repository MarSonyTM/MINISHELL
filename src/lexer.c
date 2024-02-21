#include "../inc/minishell.h"

void free_tokens(t_token **tokens)
 {
    t_token *current = *tokens;
    while (current != NULL) {
        t_token *temp = current;
        current = current->next;
        free(temp->value);
        free(temp);
    }
    *tokens = NULL;
}


void add_token(t_token **tokens, t_token_type type, char *value) {
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token) return; // Always check malloc return

    new_token->type = type;
    new_token->value = strdup(value); // Copy string to ensure independence
    new_token->next = NULL;

    if (*tokens == NULL) {
        *tokens = new_token;
    } else {
        t_token *temp = *tokens;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_token;
    }
}

void lexer(char *input, t_token **tokens) {
    char *token_start = NULL;
    int i = 0, in_single_quote = 0, in_double_quote = 0;

    while (input[i] != '\0') {
        if (input[i] == '\'' && !in_double_quote) {
            if (!in_single_quote && !token_start) {
                token_start = &input[i + 1]; // Start after quote
            } else if (in_single_quote) {
                add_token(tokens, TOKEN_ARG, strndup(token_start, &input[i] - token_start));
                token_start = NULL;
            }
            in_single_quote = !in_single_quote;
        } else if (input[i] == '\"' && !in_single_quote) {
            if (!in_double_quote && !token_start) {
                token_start = &input[i + 1]; // Start after quote
            } else if (in_double_quote) {
                add_token(tokens, TOKEN_ARG, strndup(token_start, &input[i] - token_start));
                token_start = NULL;
            }
            in_double_quote = !in_double_quote;
        } else if ((input[i] == ' ' && !in_single_quote && !in_double_quote && token_start) || (input[i + 1] == '\0' && token_start)) {
            // End of token
            int end_offset = (input[i + 1] == '\0' && input[i] != ' ') ? 1 : 0;
            if (end_offset || input[i] == ' ') {
                add_token(tokens, TOKEN_ARG, strndup(token_start, &input[i] - token_start + end_offset));
                token_start = NULL;
            }
        } else if (!token_start && !in_single_quote && !in_double_quote && input[i] != ' ') {
            // Start of a new token
            token_start = &input[i];
        }
        i++;
    }
}
