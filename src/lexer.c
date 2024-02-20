#include "../inc/minishell.h"

void free_lexer(t_lexer *head) 
{
    while (head != NULL) {
        t_lexer *temp = head;
        head = head->next;
        free(temp->value);  // Free the value field
        free(temp);
    }
}

// Helper function to check if a character is a special character
int is_special_char(char c)
{
    return (c == '<' || c == '>' || c == '|' || c == '&');
}

int get_special_char_type(char c)
{
    if (c == '<')
        return INPUT;
    else if (c == '>')
        return OUTPUT;
    else if (c == '|')
        return PIPE;
    // Add more special characters if needed
    else
        return CMD;
}

// Helper function to check if a character is a whitespace character
int is_whitespace(char c)
{
    return (c == ' ' || c == '\t');
}
t_lexer *lex(const char *input)
{
    t_lexer *head = NULL;
    t_lexer *current = NULL;
    int in_quote = 0;

    char *input_copy = strdup(input);
    char *token = strtok(input_copy, " ");
    while (token != NULL)
    {
        // Create a new lexer node
        t_lexer *node = malloc(sizeof(t_lexer));
        if (!node)
        {
            perror("Error: malloc failed");
            exit(1);
        }

        // Initialize the node
        node->value = strdup(token);
        node->type = is_special_char(token[0]) ? get_special_char_type(token[0]) : CMD;
        node->quoted = in_quote;
        node->next = NULL;

        // Handle quotes
        if (token[0] == '"')
        {
            in_quote = !in_quote;
            // Remove quotes from the token value
            memmove(node->value, node->value + 1, strlen(node->value));
            node->value[strlen(node->value) - 1] = '\0';
        }

        // Link the node to the list
        if (head == NULL)
        {
            head = node;
            current = node;
        }
        else
        {
            current->next = node;
            current = node;
        }

        // Move to the next token
        token = strtok(NULL, " ");
    }

    return head;
}

void print_command_structure(t_lexer *head)
{
    t_lexer *current = head;

    printf("Command Structure:\n");
    while (current != NULL)
    {
        if (current->type == CMD)
        {
            printf("Command: %s\n", current->value);
        }
        else if (current->type == INPUT || current->type == OUTPUT || current->type == APPEND_OUTPUT || current->type == PIPE)
        {
            printf("Command: %s\n", current->value);
            current = current->next;
            if (current != NULL && current->type == CMD)
            {
                printf("Command Structure:\n");
            }
        }
        current = current->next;
    }
}


