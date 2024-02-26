#include "../inc/minishell.h"

 
void lexer(char *input, t_token **tokens)
{
    char *token;
    const char delimiters[] = " \t\n"; // Whitespace characters used as delimiters

    // Tokenize the input string using strtok
    token = ft_strtok(input, delimiters);
    
    while (token != NULL)
    {
        if (ft_strcmp(token, "echo") == 0 ||
            ft_strcmp(token, "cd") == 0 ||
            ft_strcmp(token, "pwd") == 0 ||
            ft_strcmp(token, "export") == 0 ||
            ft_strcmp(token, "unset") == 0 ||
            ft_strcmp(token, "env") == 0 ||
            ft_strcmp(token, "exit") == 0) 
            {
            add_token(tokens, TOKEN_COMMAND, ft_strdup(token));
        } 
        else if (ft_strcmp(token, "$?") == 0)
        {
            add_token(tokens, TOKEN_EXIT_STATUS, ft_strdup(token));
        } 
        else if (token[0] == '$')
        {
            add_token(tokens, TOKEN_ENV_VAR, ft_strdup(token));
        } 
        else if (ft_strcmp(token, "|") == 0) 
        {
            add_token(tokens, TOKEN_PIPE, ft_strdup(token));
        } 
        else if (ft_strcmp(token, "<") == 0)
        {
            add_token(tokens, TOKEN_REDIRECT_IN, ft_strdup(token));
        } 
        else if (ft_strcmp(token, ">") == 0) 
        {
            add_token(tokens, TOKEN_REDIRECT_OUT, ft_strdup(token));
        } 
        else if (ft_strcmp(token, "<<hi") == 0) 
        {
            add_token(tokens, TOKEN_HEREDOC, ft_strdup(token));
            token = ft_strtok(NULL, delimiters); // Read the delimiter
            if (token != NULL) 
            {
                char *value = ft_strdup(token);
                while (ft_strcmp(token, value) != 0) 
                {
                    token = ft_strtok(NULL, delimiters);
                    if (token == NULL)
                     {
                        // Error: Unterminated delimiter
                        free(value);
                        printf("Error: Unterminated delimiter\n");
                        return;
                    }
                    char *temp = ft_strdup(value);
                    value = malloc(ft_strlen(temp) + ft_strlen(token) + 2);
                    if (!value)
                        return ;
                    ft_strcpy(value, temp);
                    ft_strcat(value, " ");
                    ft_strcat(value, token);
                    free(temp);
                }
                add_token(tokens, TOKEN_ARG, value);
            }
        } else if (ft_strcmp(token, ">>") == 0) 
        {
            add_token(tokens, TOKEN_REDIRECT_OUT_APPEND, ft_strdup(token));
        } else if (token[0] == '\'') 
        {
            // Handle single quoted strings
            char *value = ft_strdup(token);
            while (token[ft_strlen(token) - 1] != '\'') 
            {
                token = ft_strtok(NULL, delimiters);
                if (token == NULL)
                 {
                    // Error: Unterminated single quoted string
                    free(value);
                    printf("Error: Unterminated single quoted string\n");
                    return;
                }
                char *temp = ft_strdup(value);
                value = malloc(ft_strlen(temp) + ft_strlen(token) + 2);
                if (!value)
                    return ;
                ft_strcpy(value, temp);
                ft_strcat(value, " ");
                ft_strcat(value, token);
                free(temp);
            }
            add_token(tokens, TOKEN_QUOTE, value);
        } else if (token[0] == '\"') 
        {
            // Handle double quoted strings
            char *value = ft_strdup(token);
            while (token[ft_strlen(token) - 1] != '\"') 
            {
                token = ft_strtok(NULL, delimiters);
                if (token == NULL)
                 {
                    // Error: Unterminated double quoted string
                    free(value);
                    printf("Error: Unterminated double quoted string\n");
                    return;
                }
                char *temp = ft_strdup(value);
                value = malloc(ft_strlen(temp) + ft_strlen(token) + 2);
                if (!value)
                    return ;
                ft_strcpy(value, temp);
                ft_strcat(value, " ");
                ft_strcat(value, token);
                free(temp);
            }
            add_token(tokens, TOKEN_DQUOTE, value);
        } 
        else
        {
            add_token(tokens, TOKEN_ARG, ft_strdup(token));
        }

        // Get the next token
        token = ft_strtok(NULL, delimiters);
    }
}