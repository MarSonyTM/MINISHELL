#include "../inc/minishell.h"

void print_commands(t_cmd *cmd) 
{
    t_cmd *current_cmd = cmd;
    int i = 1;
    while (current_cmd != NULL) 
    {
        printf("PARSER: Command: %s\n", current_cmd->cmd_arr[0]); // Print command
        while ( current_cmd->cmd_arr[i] != NULL) 
        {
            printf("PARSER: Arg: %s\n", current_cmd->cmd_arr[i]); // Print arguments
            i++;
        }
        if (current_cmd->input != NULL)
            printf("PARSER: Input Redirection: %s\n", current_cmd->input);
        if (current_cmd->output != NULL)
            printf("PARSER: Output Redirection: %s\n", current_cmd->output);
        current_cmd = current_cmd->next; // Move to the next command
    }
}

void free_cmds(t_cmd **cmd) 
{
    int i = 0;
    t_cmd *current = *cmd;
    while (current != NULL) 
    {
        t_cmd *temp = current;
        current = current->next;
        // Free command path
        free(temp->cmd_path);
        // Free command arguments
        while(temp->cmd_arr[i] != NULL) 
        {
            free(temp->cmd_arr[i]);
            i++;
        }
        // Free input and output redirection
        free(temp->input);
        free(temp->output);
        // Free the command structure itself
        free(temp);
    }
    // Set the pointer to NULL after freeing all commands
    *cmd = NULL;
}


int main(int argc, char **argv)
{
    if (argc > 1 || argv[1] != NULL)
        exit(printf("This program takes no arguments\n"));

    while (1)
    {
        char *input = readline(PROMPT);
        if (!input)
        {
            free(input);
            printf("exit\n");
            break ;
        }
        add_history(input);
        // check if user wants to exit
        if (ft_strcmp(input, "exit") == 0)
        {
            printf("exit\n");
            free(input);
            break;
        }

        t_token *tokens = NULL; 
        lexer(input, &tokens); // Tokenize the input
     
        t_token *current = tokens;
        while (current != NULL)
        {
            const char *type_str;
            switch (current->type)
            {
                case TOKEN_ARG:
                    type_str = "Argument";
                    break;
                case TOKEN_PIPE:
                    type_str = "Pipe";
                    break;
                case TOKEN_REDIRECT_IN:
                    type_str = "Input Redirection";
                    break;
                case TOKEN_REDIRECT_OUT:
                    type_str = "Output Redirection";
                    break;
                case TOKEN_DOUBLE_REDIRECT_OUT:
                    type_str = "Double Output Redirection";
                    break;
                case TOKEN_QUOTE:
                    type_str = "Single Quote";
                    break;
                case TOKEN_DQUOTE:
                    type_str = "Double Quote";
                    break;
                case TOKEN_COMMAND:
                    type_str = "Command";
                    break;
                case TOKEN_ENV_VAR:
                    type_str = "Environment Variable";
                    break;
                case TOKEN_EXIT_STATUS:
                    type_str = "Exit Status";
                    break;
                case TOKEN_HEREDOC:
                    type_str = "Heredoc";
                    break;
                case TOKEN_COMMA:
                    type_str = "Comma";
                    break;
                case TOKEN_REDIRECT_OUT_APPEND:
                    type_str = "Output Redirection Append";
                    break;
                default:
                    type_str = "Unknown";
                    break;
            }
            printf("LEXER: Token: Type: %s, Value: %s\n", type_str, current->value);
            current = current->next;
        }
        
        t_cmd *cmd = NULL;
        parse(tokens, &cmd); // Parse the tokens into commands
        print_commands(cmd); // Print the commands
   
        
        // Print each token and its type
        free_tokens(&tokens);
        free(input);
        free_cmds(&cmd);
	}
	return (0);
}
