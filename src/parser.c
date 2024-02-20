#include "../inc/minishell.h"


void free_parser(t_parse *head) 
{
    while (head != NULL) {
        t_parse *temp = head;
        head = head->next;
        free(temp->cmd_path);  // Free the cmd_path field
        // ... (free other fields if applicable)
        free(temp);
    }
}


t_parse *parse(t_lexer *tokens) 
{
    t_parse *head = NULL;
    t_parse *current = NULL;

    while (tokens != NULL) {
        // Create a new parse node for the command
        t_parse *node = malloc(sizeof(t_parse));
        if (!node) {
            perror("Error: malloc failed");
            exit(1);
        }

        // Initialize the node
        node->cmd_path = strdup(tokens->value);
        node->cmd_arr = NULL;
        node->input = NULL;
        node->output = NULL;
        node->exit_status = 0;
        node->next = NULL;

        // Print parsed information (for now)
        printf("Parsed Token: Type %d, Value: %s\n", tokens->type, tokens->value);

        // Link the node to the list
        if (head == NULL) {
            head = node;
            current = node;
        } else {
            current->next = node;
            current = node;
        }

        // Print the command structure
        printf("Command Structure:\n");
        t_parse *temp = head;
        while (temp != NULL) {
            printf("Command: %s\n", temp->cmd_path);
            // ... (print other command components if applicable)
            temp = temp->next;
        }

        // Move to the next token
        tokens = tokens->next;
    }

    return head;
}
