/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:21:44 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/30 17:03:09 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int	g_exit_code;

/// @brief Checks if the token list contains any heredoc tokens
/// @param lst Pointer to the first token in the list to traverse
/// @return 1 if any HEREDOC token type is found in the list, 0 otherwise
static int	has_heredoc(t_token *lst)
{
	while (lst)
	{
		if (lst->type == HEREDOC)
			return (1);
		lst = lst->next;
	}
	return (0);
}

/// @brief Handles heredoc interruption and syntax error checking
/// @param lst Pointer to the token list to be checked for errors
/// @param tokens Array of token info structures to be freed on error
/// @param token_count Number of tokens in the array for proper cleanup
/// @param env Pointer to the environment AST where error codes are stored
/// @return token_count on success, 0 on error (sets appropriate error code)
static int	handle_heredoc_and_syntax(t_token **lst, t_token_info **tokens,
		int token_count, t_ast **env)
{
	if (has_heredoc(*lst) && g_exit_code == 130)
	{
		(*env)->env->error_code = 130;
		g_exit_code = 0;
		free_token_info_array(*tokens, token_count);
		*tokens = NULL;
		*lst = NULL;
		return (0);
	}
	if (check_syntax_errors(*lst))
	{
		(*env)->env->error_code = 2;
		free_token_info_array(*tokens, token_count);
		*tokens = NULL;
		*lst = NULL;
		return (0);
	}
	return (token_count);
}

/// @brief Initializes and processes tokens from input line,
///	handles heredoc and syntax checking
/// @param lst Pointer to token list to be populated
/// @param line Input command line string
/// @param env Pointer to environment AST for variable expansion
/// @param tokens Pointer to token info array to be allocated
/// @return Number of tokens on success, 0 on error or interruption
static int	init_tokens_and_check(t_token **lst, char *line, t_ast **env,
		t_token_info **tokens)
{
	int		token_count;
	char	*expanded_line;
	char	*heredoc_path;
	int		result;
	t_token	*original_list;

	expanded_line = unquoted_var_expansion(line, env);
	*tokens = split_bash_style_with_quotes(expanded_line, &token_count);
	if (expanded_line != line)
		free(expanded_line);
	*lst = NULL;
	create_list_with_quote_info(lst, *tokens, token_count);
	original_list = *lst;
	exit_status(lst, env);
	heredoc_path = NULL;
	check_heredoc(lst, env, &heredoc_path);
	if (heredoc_path)
		free(heredoc_path);
	result = handle_heredoc_and_syntax(lst, tokens, token_count, env);
	if (result == 0 && original_list)
	{
		free_stack(&original_list);
	}
	return (result);
}

/// @brief Builds AST from tokens and executes the parsed command(s)
/// @param lst Pointer to token list to be parsed
/// @param env Pointer to environment AST
/// @param tokens Token info array to be freed after use
/// @param token_count Number of tokens in the array
static void	handle_ast_and_exec(t_token **lst, t_ast **env,
		t_token_info *tokens, int token_count)
{
	t_ast	*ast_root;
	t_token	*original_list_head;

	if (!*lst)
		return ;
	original_list_head = *lst;
	exit_status(lst, env);
	ast_root = parse_pipeline(lst, env);
	if (tokens)
		free_token_info_array(tokens, token_count);
	free_stack(&original_list_head);
	if (ast_root)
	{
		execute_nodes(&ast_root, env);
		ast_free(ast_root);
	}
	unlink_redirection(&original_list_head);
}

/// @brief Main token processing function
///	- parses input line and executes commands
/// @param lst Pointer to token list (will be populated then used for execution)
/// @param line Input command line string to be processed
/// @param env Pointer to environment AST for variable expansion and
/// execution context
void	process_tokens(t_token **lst, char *line, t_ast **env)
{
	t_token_info	*tokens;
	int				token_count;

	token_count = init_tokens_and_check(lst, line, env, &tokens);
	handle_ast_and_exec(lst, env, tokens, token_count);
}
