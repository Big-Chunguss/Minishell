/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_ast4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:14:09 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/22 16:26:39 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	define_type_from_token(t_token *token, t_ast **env)
{
	char	*cmd_path;

	if (token->was_quoted && (!strcmp(token->value, "|")
			|| !strcmp(token->value, "<") || !strcmp(token->value, ">")
			|| !strcmp(token->value, ">>") || !strcmp(token->value, "<<")))
		return (NODE_ARGUMENT);
	cmd_path = get_cmd_path(token->value, env);
	if (cmd_path)
	{
		free(cmd_path);
		return (NODE_COMMAND);
	}
	if (!strcmp(token->value, "<<") || !strcmp(token->value, "<")
		|| !strcmp(token->value, ">>") || !strcmp(token->value, ">"))
		return (NODE_REDIRECTION);
	if (!strcmp(token->value, "|"))
		return (NODE_PIPE);
	return (NODE_ARGUMENT);
}

void	set_redirection_target(t_ast *redir, t_ast *target)
{
	redir->target = target;
}

static t_ast	*parse_command_head(t_token **lst_ptr, t_ast **env)
{
	t_token	*lst;
	t_ast	*cmd;
	bool	has_redirections;

	lst = *lst_ptr;
	cmd = NULL;
	has_redirections = false;
	while (lst && define_type_from_token(lst, env) != NODE_PIPE)
	{
		if (define_type_from_token(lst, env) != NODE_REDIRECTION)
		{
			cmd = create_ast_node(NODE_COMMAND, lst->value);
			lst = lst->next;
			break ;
		}
		has_redirections = true;
		lst = lst->next;
		if (lst)
			lst = lst->next;
	}
	
	// If we have redirections but no command, create a pass-through command
	if (!cmd && has_redirections)
	{
		cmd = create_ast_node(NODE_COMMAND, "");
	}
	*lst_ptr = lst;
	return (cmd);
}

static void	parse_command_args(t_ast *cmd, t_token **lst_ptr, t_ast **env)
{
	t_token	*lst;

	lst = *lst_ptr;
	while (lst && define_type_from_token(lst, env) != NODE_PIPE)
	{
		if (define_type_from_token(lst, env) != NODE_REDIRECTION)
		{
			add_ast_child(cmd, create_ast_node(NODE_ARGUMENT, lst->value));
			lst = lst->next;
		}
		else
		{
			lst = lst->next;
			if (lst)
				lst = lst->next;
		}
	}
	*lst_ptr = lst;
}

t_ast	*parse_command(t_token **lst_ptr, t_ast **env)
{
	t_ast	*cmd;

	cmd = parse_command_head(lst_ptr, env);
	parse_command_args(cmd, lst_ptr, env);
	return (cmd);
}
