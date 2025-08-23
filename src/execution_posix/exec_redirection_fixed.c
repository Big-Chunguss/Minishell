/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection_fixed.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:33:49 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 11:48:02 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

static int	print_redir_error(const char *path)
{
	char	buf[512];
	char	*msg;
	size_t	len;
	size_t	i;

	if (errno == EACCES)
		msg = "Permission denied";
	else if (errno == ENOENT)
		msg = "No such file or directory";
	else
		msg = strerror(errno);
	len = 0;
	if (path)
	{
		i = 0;
		while (path[i] && len < sizeof(buf) - 1)
			buf[len++] = path[i++];
	}
	if (len < sizeof(buf) - 1)
		buf[len++] = ':';
	if (len < sizeof(buf) - 1)
		buf[len++] = ' ';
	i = 0;
	while (msg[i] && len < sizeof(buf) - 1)
		buf[len++] = msg[i++];
	if (len < sizeof(buf) - 1)
		buf[len++] = '\n';
	write(2, buf, len);
	return (-1);
}

static int	open_output_redir(t_ast *redir)
{
	int	fd;

	if (!ft_strcmp(redir->value, ">"))
		fd = open(redir->target->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->target->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (print_redir_error(redir->target->value));
	return (fd);
}

static int	open_input_redir(t_ast *redir)
{
	int	fd;

	fd = open(redir->target->value, O_RDONLY);
	if (fd < 0)
		return (print_redir_error(redir->target->value));
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
	char	*heredoc_path;

	if (g_exit_code == 130)
		return (-1);
	if (!ft_strcmp(child->value, "<"))
		return (handle_input_redirection(child));
	else if (!ft_strcmp(child->value, "<<"))
	{
		heredoc_path = find_heredoc_file(child->target->value);
		if (!heredoc_path)
		{
			write(2, "No heredoc file found\n", 22);
			return (-1);
		}
		free(child->target->value);
		child->target->value = heredoc_path;
		return (handle_input_redirection(child));
	}
	else if (!ft_strcmp(child->value, ">") || !ft_strcmp(child->value, ">>"))
		return (handle_output_redirection(child));
	return (0);
}
