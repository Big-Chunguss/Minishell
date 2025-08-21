/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:33:49 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/21 19:45:23 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	open_output_redir(t_ast *redir)
{
	int	fd;

	if (!ft_strcmp(redir->value, ">"))
		fd = open(redir->target->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->target->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		handle_errno_error(redir->target->value);
	return (fd);
}

/// @brief checking if node contains an input redirection and applying
/// it to STDIN
/// @param node
static int	open_input_redir(t_ast *redir)
{
	int	fd;

	fd = open(redir->target->value, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}
	return (fd);
}

static int	handle_input_redirection(t_ast *child)
{
	int	fd;

	fd = open_input_redir(child);
	if (fd == -1)
		return (-1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	handle_output_redirection(t_ast *child)
{
	int	fd;

	fd = open_output_redir(child);
	if (fd == -1)
		return (-1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	process_redirection_child(t_ast *child)
{
	if (g_exit_code == 130)
		return (-1);
	if (!strcmp(child->value, "<"))
		return (handle_input_redirection(child));
	else if (!strcmp(child->value, "<<"))
	{
		if (access(child->target->value, F_OK) == -1)
			return (-1);
		return (handle_input_redirection(child));
	}
	else if (!strcmp(child->value, ">") || !strcmp(child->value, ">>"))
		return (handle_output_redirection(child));
	return (0);
}
