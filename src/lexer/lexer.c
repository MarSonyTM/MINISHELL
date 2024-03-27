#include "../../inc/minishell.h"

int	lexer(char *input, t_token **tokens, t_env *env) 
{
	int		i; // Index for the input string
	char *buffer; // Buffer to store the current token
	int TokenCount; // Number of tokens
	int bufIndex; // Index for the buffer
	int inQuote; // Flag to indicate if we are in a quote
	bool quote_error; // Flag to indicate if there was a quote error
	i = 0; // Initialize the index to 0
	buffer = malloc((ft_strlen(input) + 1) * sizeof(char)); // Allocate memory for the buffer
	TokenCount = 0; // Initialize the token count to 0
	bufIndex = 0; // Initialize the buffer index to 0
	inQuote = 0; // Initialize the inQuote flag to 0
	quote_error = false; // Initialize the quote error flag to false

    if (buffer == NULL) // Check for allocation error
        return (1);

    // Process the input string
    process_input_loop(input, &buffer, &bufIndex, &tokens, &TokenCount, env, &i, &inQuote, &quote_error);
    // Finalize the lexer
	return (finalize_lexer(&buffer, bufIndex, &tokens, &TokenCount, inQuote, quote_error, env));
}

