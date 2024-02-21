#include "../inc/minishell.h"


int main(int argc, char **argv) {
    if (argc != 1 || argv[1])
        exit(printf("This program takes no arguments\n"));

    t_token *tokens = NULL; // Initialize token list

    while (1) {
        char *input = readline(PROMPT);
        if (!input)
            break;
        if (strcmp(input, "exit") == 0) {
            free(input);
            break;
        }
        add_history(input);
        
        lexer(input, &tokens); // Tokenize the input

        // Here you would typically parse and execute based on tokens
        // For now, let's just print them to verify
        t_token *current = tokens;
        while (current != NULL) {
            printf("Token: Type: %d, Value: %s\n", current->type, current->value);
            current = current->next;
        }
        // Remember to free tokens after use
        free_tokens(&tokens);
        free(input);
    }
    return 0;
}
