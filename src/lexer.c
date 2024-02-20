#include "../inc/minishell.h"

// Helper function to check if a token is a special character
int is_special_char(char c) 
{
    return (c == '<' || c == '>' || c == '|' || c == '&');
}

void free_lexer(t_lexer *head) 
{
    while (head != NULL) {
        t_lexer *temp = head;
        head = head->next;
        free(temp->value);  // Free the value field
        free(temp);
    }
}

t_lexer *lex(const char *input)
 {
    t_lexer *head = NULL;
    t_lexer *current = NULL;

    // Tokenize based on spaces and special characters
    char *input_copy = strdup(input);  // Duplicate the input to avoid modifying the original string
    char *token = strtok(input_copy, " ");
    while (token != NULL) 
    {
        // Create a new lexer node
        t_lexer *node = malloc(sizeof(t_lexer));
        if (!node) {
            // Handle malloc failure
            perror("Error: malloc failed");
            exit(1);
        }

        // Initialize the node
        node->value = strdup(token);  // store the token value

        // Check for special characters
        if (strcmp(token, "<") == 0)
        	node->type = INPUT;
        else if (strcmp(token, ">") == 0)
        	node->type = OUTPUT;
        else if (strcmp(token, ">>") == 0)
        	node->type = APPEND_OUTPUT;
        else if (strcmp(token, "|") == 0)
        	node->type = PIPE;
        else 
        	node->type = CMD;

node->next = NULL;

        // Link the node to the list
        if (head == NULL) {
            head = node;
            current = node;
        } else {
            current->next = node;
            current = node;
        }

        printf("Token: %s, Type: %d\n", node->value, node->type);

        // Move to the next token
        token = strtok(NULL, " ");
    }

    return head;
}
