#include "../inc/minishell.h"

void print_parsed_structure(t_parse *head) {
    printf("Final Parsed Structure:\n");
    t_parse *temp = head;
    while (temp != NULL) {
        printf("Command: %s\n", temp->command->cmd_path);
        temp = temp->next;
    }
}

void free_parser(t_parse *head) 
{
    while (head != NULL) {
        t_parse *temp = head;
        head = head->next;

        // Free the command structure
        free(temp->command->cmd_path);
        // ... (free other fields in the command structure if applicable)
        free(temp->command);

        free(temp);
    }
}

t_parse *parse(t_lexer *tokens) {
    t_parse *head = NULL;
    t_parse *current = NULL;

    // Create a temporary command structure outside the loop
    t_command *command = NULL;

    while (tokens != NULL) {
        if (tokens->type == CMD) {
            // Only create a new command structure if the token is a command
            command = malloc(sizeof(t_command));
            if (!command) {
                perror("Error: malloc failed");
                exit(1);
            }

            // Initialize the command structure
            command->cmd_path = strdup(tokens->value);
            command->cmd_args = NULL;
            command->input_file = NULL;
            command->output_file = NULL;
            command->append_output = 0;
        }

        // Create a new parse node
        t_parse *node = malloc(sizeof(t_parse));
        if (!node) {
            perror("Error: malloc failed");
            exit(1);
        }

        // Initialize the node
        node->command = command;
        node->exit_status = 0;
        node->next = NULL;

        // Link the node to the list
        if (head == NULL) {
            head = node;
            current = node;
        } else {
            current->next = node;
            current = node;
        }

        print_parsed_structure(head);

        // Move to the next token
        tokens = tokens->next;
    }

    return head;
}
