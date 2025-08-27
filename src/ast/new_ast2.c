/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_ast2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:04:04 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 22:29:10 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	define_type(char *str, t_ast **env)
{
	char	*cmd_path;

	cmd_path = get_cmd_path(str, env);
	if (cmd_path)
	{
		free(cmd_path);
		return (NODE_COMMAND);
	}
	if (!ft_strcmp(str, "<<") || !ft_strcmp(str, "<") || !ft_strcmp(str, ">>")
		|| !ft_strcmp(str, ">"))
		return (NODE_REDIRECTION);
	if (!ft_strcmp(str, "|"))
		return (NODE_PIPE);
	return (NODE_ARGUMENT);
}

void	ast_free(t_ast *node)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	if (node->value)
		free(node->value);
	while (i < node->child_count)
		ast_free(node->children[i++]);
	if (node->children)
		free(node->children);
	if (node->left)
		ast_free(node->left);
	if (node->right)
		ast_free(node->right);
	if (node->target)
		ast_free(node->target);
	if (node)
	{
		free(node);
		node = NULL;
	}
}

void	set_ast_left(t_ast *parent, t_ast *child)
{
	parent->left = child;
	if (child)
		child->parent = parent;
}

void	set_ast_right(t_ast *parent, t_ast *child)
{
	parent->right = child;
	if (child)
		child->parent = parent;
}

void	add_redirection(t_ast *cmd, t_ast *redir)
{
	add_ast_child(cmd, redir);
}
