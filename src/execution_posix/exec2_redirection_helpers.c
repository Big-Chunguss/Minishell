/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_redirection_helpers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:30:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 16:30:00 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int	is_input_redirection(t_ast *node)
{
	if (node && node->value)
	{
		return (ft_strcmp(node->value, "<") == 0
			|| ft_strcmp(node->value, "<<") == 0);
	}
	return (0);
}

int	is_heredoc_redirection(t_ast *node)
{
	if (node && node->value)
	{
		return (ft_strcmp(node->value, "<<") == 0);
	}
	return (0);
}

int	find_last_heredoc_redirection(t_ast *node)
{
	int	i;
	int	last_heredoc;

	if (!node)
		return (-1);
	last_heredoc = -1;
	i = 0;
	while (i < node->child_count)
	{
		if (node->children[i]->type == NODE_REDIRECTION
			&& is_heredoc_redirection(node->children[i]))
			last_heredoc = i;
		i++;
	}
	return (last_heredoc);
}

int	process_single_redirection(t_ast *node, int i, int last_heredoc_redir)
{
	int	res;

	if (is_heredoc_redirection(node->children[i]))
	{
		if (i == last_heredoc_redir)
		{
			res = process_redirection_child(node->children[i]);
			if (res == -1)
				return (-1);
		}
	}
	else
	{
		res = process_redirection_child(node->children[i]);
		if (res == -1)
			return (-1);
	}
	return (0);
}
