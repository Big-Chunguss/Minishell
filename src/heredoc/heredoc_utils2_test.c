/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2_test.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:41:51 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 21:00:59 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	convert_pid_to_string_reversed(int pid, char *pid_str)
{
	int	i;

	i = 0;
	if (pid == 0)
		pid_str[i++] = '0';
	else
	{
		while (pid > 0)
		{
			pid_str[i++] = (pid % 10) + '0';
			pid /= 10;
		}
	}
	pid_str[i] = '\0';
}

void	clean_heredoc(char **argv)
{
	char	*temp_path;
	char	*pid_str;

	temp_path = malloc(256);
	pid_str = malloc(16);
	if (!temp_path || !pid_str)
	{
		if (temp_path)
			free(temp_path);
		if (pid_str)
			free(pid_str);
		return ;
	}
	if (!ft_strcmp(argv[1], "here_doc"))
		free(argv[1]);
	get_next_line(-1);
	convert_pid_to_string_reversed(getpid(), pid_str);
	ft_strlcpy(temp_path, "/tmp/minishell_heredoc_*_", 256);
	ft_strlcat(temp_path, pid_str, 256);
	free(temp_path);
	free(pid_str);
}

