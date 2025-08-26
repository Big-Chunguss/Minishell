/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:21:44 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/26 12:35:34 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int	g_exit_code;

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

static int	handle_heredoc_and_syntax(t_token **lst, t_token_info **tokens,
		int token_count, t_ast **env)
{
	if (has_heredoc(*lst) && g_exit_code == 130)
	{
		(*env)->env->error_code = 130;
		g_exit_code = 0;
		free_token_info_array(*tokens, token_count);
		free_stack(lst);
		return (0);
	}
	if (check_syntax_errors(*lst))
	{
		(*env)->env->error_code = 2;
		free_token_info_array(*tokens, token_count);
		free_stack(lst);
		return (0);
	}
	return (token_count);
}

static int	init_tokens_and_check(t_token **lst, char *line, t_ast **env,
		t_token_info **tokens)
{
	int		token_count;
	char	*original_line;

	original_line = line;
	line = unquoted_var_expansion(line, env);
	*tokens = split_bash_style_with_quotes(line, &token_count);
	if (line != original_line)
		free(line);
	*lst = NULL;
	create_list_with_quote_info(lst, *tokens, token_count);
	check_heredoc(lst, env);
	return (handle_heredoc_and_syntax(lst, tokens, token_count, env));
}

static void	handle_ast_and_exec(t_token **lst, t_ast **env,
		t_token_info *tokens, int token_count)
{
	t_ast	*ast_root;
	t_token	*original_list_head;

	original_list_head = *lst;
	exit_status(lst, env);
	ast_root = parse_pipeline(lst, env);
	unlink_redirection(&original_list_head);
	free_token_info_array(tokens, token_count);
	free_stack(&original_list_head);
	if (ast_root)
	{
		execute_nodes2(&ast_root, env);
		ast_free(ast_root);
	}
}

void	process_tokens(t_token **lst, char *line, t_ast **env)
{
	t_token_info	*tokens;
	int				token_count;

	token_count = init_tokens_and_check(lst, line, env, &tokens);
	if (token_count == 0)
		return ;
	handle_ast_and_exec(lst, env, tokens, token_count);
}
