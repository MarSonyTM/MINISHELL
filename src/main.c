/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:51:56 by csturm            #+#    #+#             */
/*   Updated: 2024/04/18 18:38:01 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


void	print_commands(t_cmd *cmd)
{
	t_cmd	*current_cmd = cmd;
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
        if (current_cmd->redirection_append != NULL)
            printf("PARSER: Output Redirection Append: %s\n", current_cmd->redirection_append);
        current_cmd = current_cmd->next; // Move to the next commandr
        i = 1; // Reset argument index for the next command
    }
}


char *append_to_string(char *str, const char *append)
{
    char *new_str;
    size_t str_len = str ? ft_strlen(str) : 0;
    size_t append_len = append ? ft_strlen(append) : 0;

    new_str = malloc(str_len + append_len + 1);
    if (new_str == NULL)
    {
        // Handle the error.
        return NULL;
    }

    if (str)
    {
        ft_strcpy(new_str, str);
    }

    if (append)
    {
        ft_strcpy(new_str + str_len, append);
    }

    new_str[str_len + append_len] = '\0';

    free(str);

    return new_str;
} 
void expand_env_varss(char **input, t_env *env)
{
    char *result = NULL;
    char *cursor = *input;
    int in_single_quote = 0;
    int in_double_quote = 0;
    while (*cursor) {
        if (*cursor == '\"')
		{
            // Toggle in_double_quote status on encountering a double quote
            in_double_quote = !in_double_quote;
            // Add the double quote to the result
            result = append_to_string(result, "\"");
            cursor++;
            continue;
        } else if (*cursor == '\'')
		{
            // Toggle in_single_quote status on encountering a single quote
            in_single_quote = !in_single_quote;
            // Add the single quote to the result
            result = append_to_string(result, "'");
            cursor++;
            continue;
        }

        if (*cursor == '$' && !in_single_quote) {
    cursor++; // Skip over the '$'
    if (*cursor == '\0' || isspace(*cursor) || *cursor == '$' || (in_double_quote && (!isalnum(*cursor) && *cursor != '_'))) {
        // Handle the case where $ is the last character or followed by space or another $ or inside double quotes and not followed by a variable name
        result = append_to_string(result, "$");
        if (*cursor != '\0' && *cursor != '\"' && *cursor != '\'') cursor++;
        continue;
    }

            // Find the end of the variable name
            char *end = cursor;
            while (*end && (isalnum(*end) || *end == '_'))
			{
                end++;
            }

            // Extract variable name
            char *var_name = ft_strndup(cursor, end - cursor);
            char *var_value = get_env_value(var_name, env);
            free(var_name);

            // Append variable value to result
            if (var_value)
			{
                result = append_to_string(result, var_value);
                free(var_value);
            }

            cursor = end;
        } else
		{
            // Handle normal characters
            char append[2] = {*cursor, '\0'};
            result = append_to_string(result, append);
            cursor++;
        }
    }
    free(*input);
    *input = result;
}

int	handle_lexer(int lexer_status, t_token **tokens, char **input)
{
	if (lexer_status == 1)
	{
		free_tokens(tokens);
		free(*input);
		exit(1);
	}
	else if (lexer_status == 2)
	{
		free_tokens(tokens);
		free(*input);
		return (1);
	}
	return (0);
}

int	handle_parser(int parse_status, t_cmd **cmd, t_token **tokens, char **input)
{
	if (parse_status == 1)
	{
		free_cmds(cmd);
		free_tokens(tokens);
		free(*input);
		exit(1);
	}
	else if (parse_status == 2)
	{
		free_cmds(cmd);
		free_tokens(tokens);
		free(*input);
		return (1);
	}
	return (0);
}

void	main_handle_input(char **input)
{
	*input = readline(PROMPT);
	sleep(0);
	if (!*input)
	{
		free(*input);
		ft_putendl_fd("exit", 1);
		exit(0);
	}
	add_history(*input);
}

void	main_loop(t_env **env, int *exit_status)
{
	char		*input;
	t_token		*tokens;
	t_cmd		*cmd;
	t_lexer		lexer_instance;

	 while (1)
    {
        main_handle_input(&input);
		expand_env_varss(&input, *env);
		printf("EXPAND: %s\n", input);
		tokens = NULL;
		if (handle_lexer(lexer(input, &tokens,
					&lexer_instance), &tokens, &input))
			continue ;
		t_token *current;
		current = tokens;
		const char	*type_str;
		while (current != NULL)
		{
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
				case T_R_OT:
					type_str = "Output Redirection";
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
				case TOKEN_EXIT_STATUS:
					type_str = "Exit Status";
					break;
				case TOKEN_HEREDOC:
					type_str = "Heredoc";
					break;
				case TOKEN_COMMA:
					type_str = "Comma";
					break;
				case T_R_OUT_A:
					type_str = "Output Redirection Append";
					break;
				case TOKEN_BUILTIN:
					type_str = "Builtin";
					break;
				default:
					type_str = "Unknown";
					break;
			}
			printf("LEXER: Token: Type: %s, Value: %s\n", type_str, current->value);
			current = current->next;
		}
		cmd = NULL;
		if (handle_parser(parse(tokens, &cmd, *env), &cmd, &tokens, &input))
		{
			*exit_status = 127;
			continue ;
		}
		print_commands(cmd);
		free_tokens(&tokens);
		*exit_status = executor(cmd, env, *exit_status);
		reset_free_cmd(&cmd, input);
	}
}


int	main(int argc, char **argv, char **envp)
{
	t_env		*env;
	int			exit_status;

	exit_status = 0;
	check_args(argc, argv);
	init_env_signals(&env, envp);
	main_loop(&env, &exit_status);
	return (0);
}
