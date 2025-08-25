/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 14:17:17 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	convert_pid_to_string(int pid, char *pid_str)
{
	int	i;
	int	num;

	i = 0;
	num = pid;
	while (num > 0)
	{
		pid_str[i++] = (num % 10) + '0';
		num /= 10;
	}
	pid_str[i] = '\0';
}

static void	append_reversed_string(char *temp_path, char *pid_str,
		size_t path_size)
{
	int		i;
	char	*c;

	c = malloc(2);
	if (!c)
		return ;
	i = ft_strlen(pid_str);
	while (--i >= 0)
	{
		c[0] = pid_str[i];
		c[1] = '\0';
		ft_strcat_safe(temp_path, c, path_size);
	}
	free(c);
}

void	build_temp_path(char *temp_path, size_t path_size, const char *filename,
		int pid)
{
	char	*pid_str;

	pid_str = malloc(12);
	if (!pid_str)
		return ;
	ft_strcpy_safe(temp_path, "/tmp/minishell_heredoc_", path_size);
	ft_strcat_safe(temp_path, filename, path_size);
	ft_strcat_safe(temp_path, "_", path_size);
	if (pid == 0)
	{
		ft_strcat_safe(temp_path, "0", path_size);
		free(pid_str);
		return ;
	}
	convert_pid_to_string(pid, pid_str);
	append_reversed_string(temp_path, pid_str, path_size);
	free(pid_str);
}

void	build_pattern(char *pattern, size_t size, const char *trimmed_limiter)
{
	ft_strcpy_safe(pattern, "/tmp/minishell_heredoc_", size);
	ft_strcat_safe(pattern, trimmed_limiter, size);
	ft_strcat_safe(pattern, "_*", size);
}
