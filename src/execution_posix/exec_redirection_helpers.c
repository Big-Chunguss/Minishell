/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:15:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 12:56:00 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	print_redir_error(const char *path)
{
	if (errno == EACCES)
	{
		write(2, path, ft_strlen(path));
		write(2, ": Permission denied\n", 20);
	}
	else if (errno == ENOENT)
	{
		write(2, path, ft_strlen(path));
		write(2, ": No such file or directory\n", 28);
	}
	else
	{
		write(2, path, ft_strlen(path));
		write(2, ": ", 2);
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
	}
	return (-1);
}

int	open_output_redir(t_ast *redir)
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

int	open_input_redir(t_ast *redir)
{
	int	fd;

	fd = open(redir->target->value, O_RDONLY);
	if (fd < 0)
		return (print_redir_error(redir->target->value));
	return (fd);
}
