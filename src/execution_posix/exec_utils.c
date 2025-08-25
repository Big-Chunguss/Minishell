/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:00:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 12:56:00 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	command_not_found_error(const char *cmd)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", 20);
}

void	handle_errno_error(const char *path)
{
	char	*msg;

	write(2, path, ft_strlen(path));
	write(2, ": ", 2);
	if (errno == EACCES)
	{
		write(2, "Permission denied\n", 18);
		exit(1);
	}
	else if (errno == ENOENT)
	{
		write(2, "No such file or directory\n", 26);
		exit(1);
	}
	else
	{
		msg = strerror(errno);
		write(2, msg, ft_strlen(msg));
		write(2, "\n", 1);
		exit(1);
	}
}

void	exit_child(int exit_code, int child)
{
	if (child == CHILD)
		exit(exit_code);
}
