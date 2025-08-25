/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:10:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 15:05:00 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

char	**build_argv(t_ast *node)
{
	char	**tab;
	int		i;
	int		argc;

	tab = (char **)malloc(sizeof(char *) * (node->child_count + 2));
	if (!tab)
		return (NULL);
	tab[0] = node->value;
	i = 0;
	argc = 1;
	while (i < node->child_count)
	{
		if (node->children[i]->type == NODE_ARGUMENT)
			tab[argc++] = node->children[i]->value;
		i++;
	}
	tab[argc] = NULL;
	return (tab);
}

int	apply_redirections2(t_ast *node)
{
	int	i;
	int	last_heredoc_redir;

	if (!node)
		return (0);
	last_heredoc_redir = find_last_heredoc_redirection(node);
	i = 0;
	while (i < node->child_count)
	{
		if (node->children[i]->type == NODE_REDIRECTION)
		{
			if (process_single_redirection(node, i, last_heredoc_redir) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}
