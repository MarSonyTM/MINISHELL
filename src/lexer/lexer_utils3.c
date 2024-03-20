/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 13:32:40 by mafurnic          #+#    #+#             */
/*   Updated: 2024/03/20 13:32:55 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int process_whitespace(char **buffer, int *bufIndex, t_token **tokens, int inQuote) {
    if (*bufIndex > 0) {
        (*buffer)[*bufIndex] = '\0'; // Null-terminate the current token
        if (add_token(tokens, determine_token_type(*buffer, inQuote), ft_strdup(*buffer)) == 1) {
            return 1; // Handle error
        }
        *bufIndex = 0; // Reset buffer index for the next token
    }
    return 0;
}

void handle_quote(char currentChar, int *inQuote, char **buffer, int *bufIndex) {
    if (*inQuote == 0) { // Starting a quote
        *inQuote = (currentChar == '\'') ? 1 : 2;
    } else if ((*inQuote == 1 && currentChar == '\'') || (*inQuote == 2 && currentChar == '\"')) { // Ending a quote
        *inQuote = 0;
    } else { // Inside quotes, treat as part of the token
        (*buffer)[(*bufIndex)++] = currentChar;
    }
}

int process_special_character(char **buffer, int *bufIndex, t_token **tokens, char currentChar, char nextChar, int *i, int inQuote) {
    if (*bufIndex > 0) {
        (*buffer)[*bufIndex] = '\0';
        if (add_token(tokens, determine_token_type(*buffer, inQuote), ft_strdup(*buffer)) == 1) {
            return 1; // Handle error
        }
        *bufIndex = 0;
    }
    // Handling >, >>, <, <<
    if (currentChar == '<' && nextChar != '<') {
        return add_token(tokens, TOKEN_REDIRECT_IN, ft_strdup("<"));
    } else if (currentChar == '>' && nextChar != '>') {
        return add_token(tokens, TOKEN_REDIRECT_OUT, ft_strdup(">"));
    } else if (currentChar == '<' && nextChar == '<') {
        (*i)++;
        return add_token(tokens, TOKEN_HEREDOC, ft_strdup("<<"));
    } else if (currentChar == '>' && nextChar == '>') {
        (*i)++;
        return add_token(tokens, TOKEN_REDIRECT_OUT_APPEND, ft_strdup(">>"));
    } else if (currentChar == ',') {
        return add_token(tokens, TOKEN_COMMA, ft_strdup(","));
    }
    return 0;
}

int handle_variable(char **buffer, int *bufIndex, t_token **tokens, char currentChar, char nextChar, int *i, int inQuote) {
    if (currentChar == '$' && nextChar == '?') {
        if (*bufIndex > 0) { // Flush the buffer before `$?`
            (*buffer)[*bufIndex] = '\0';
            if (add_token(tokens, determine_token_type(*buffer, inQuote), ft_strdup(*buffer)) == 1) {
                return 1; // Handle error
            }
            *bufIndex = 0;
        }
        (*i)++; // Skip '?' character
        return add_token(tokens, TOKEN_EXIT_STATUS, ft_strdup("$?"));
    } else if (currentChar == '$') {
        // Handle other $ cases as needed
    }
    return 0; // No special handling occurred
}

int finalize_token_addition(char **buffer, int bufIndex, t_token **tokens, int inQuote) {
    if (bufIndex > 0) 
    {
        if (inQuote != 0) { // Unclosed quote error
            error(ERR_QUOT, NULL);
            return 1;
        } else {
            (*buffer)[bufIndex] = '\0';
            return add_token(tokens, determine_token_type(*buffer, inQuote), ft_strdup(*buffer));
        }
    }
    return 0;
}



