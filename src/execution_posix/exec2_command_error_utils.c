/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_command_error_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfragnol <hfragnol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:20:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 11:39:16 by hfragnol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	write_error_msg(const char *cmd, const char *msg)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

void	print_exec_error(const char *cmd, int code, int has_slash)
{
	if (code == 127)
	{
		if (has_slash)
			write_error_msg(cmd, "No such file or directory");
		else
			write_error_msg(cmd, "command not found");
	}
	else if (code == 126)
	{
		if (is_directory(cmd))
			write_error_msg(cmd, "Is a directory");
		else if (access(cmd, F_OK) == 0 && access(cmd, X_OK) != 0)
			write_error_msg(cmd, "Permission denied");
	}
}

int	classify_error(const char *cmd, char *resolved)
{
	int	has_slash;

	has_slash = (strchr(cmd, '/') != NULL);
	if (!resolved)
		return (127);
	if (is_directory(resolved))
		return (126);
	if (access(resolved, F_OK) == 0 && access(resolved, X_OK) != 0)
		return (126);
	return (0);
}
