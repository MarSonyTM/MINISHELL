#include "../inc/minishell.h"

int main(int argc, char **argv)
{
    if (argc != 1 || argv[1])
        exit(printf("This program takes no arguments\n"));

    while (1)
    {
        char *input = readline(PROMPT);
        if (!input)
            break;
        add_history(input);
        // check if user wants to exit
        if (strcmp(input, "exit") == 0)
        {
            printf("exit\n");
            free(input);
            break;
        }

        t_token *tokens = NULL;
        lexer(input, &tokens); // Tokenize the input
        
        // Print each token and its type
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
                case TOKEN_REDIRECT_OUT_APPEND:
                    type_str = "Output Redirection Append";
                    break;
                default:
                    type_str = "Unknown";
                    break;
            }
            printf("Token: Type: %s, Value: %s\n", type_str, current->value);
            current = current->next;
        }
        free_tokens(&tokens);
	}
	return (0);
}
