/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 17:00:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 12:37:31 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_strcpy_safe(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	while (src[i] && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

void	ft_strcat_safe(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	i;

	dst_len = ft_strlen(dst);
	i = 0;
	while (src[i] && dst_len + i < size - 1)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
}

void	build_temp_path(char *temp_path, size_t path_size, const char *filename, int pid)
{
	char	pid_str[12];
	int		i;
	int		num;

	ft_strcpy_safe(temp_path, "/tmp/minishell_heredoc_", path_size);
	ft_strcat_safe(temp_path, filename, path_size);
	ft_strcat_safe(temp_path, "_", path_size);
	
	// Convert pid to string manually
	if (pid == 0)
	{
		ft_strcat_safe(temp_path, "0", path_size);
		return;
	}
	
	i = 0;
	num = pid;
	while (num > 0)
	{
		pid_str[i++] = (num % 10) + '0';
		num /= 10;
	}
	
	// Reverse the string
	while (--i >= 0)
	{
		char c[2] = {pid_str[i], '\0'};
		ft_strcat_safe(temp_path, c, path_size);
	}
}

void	build_pattern(char *pattern, size_t size, const char *trimmed_limiter)
{
	ft_strcpy_safe(pattern, "/tmp/minishell_heredoc_", size);
	ft_strcat_safe(pattern, trimmed_limiter, size);
	ft_strcat_safe(pattern, "_*", size);
}
