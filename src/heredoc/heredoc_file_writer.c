/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file_writer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 23:10:19 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	tab_to_file(char **lines, const char *filename)
{
	char	*temp_path;
	int		fd;

	temp_path = malloc(256);
	if (!temp_path)
		return ;
	build_temp_path(temp_path, 256, filename, getpid());
	fd = open(temp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(temp_path);
		return ;
	}
	if (write_all_lines(fd, lines) == 0)
		link_or_copy_temp(temp_path, filename);
	close(fd);
	free(temp_path);
}

static int	write_to_temp_file(char **lines, const char *filename,
		char *temp_path)
{
	int	fd;

	fd = open_tempfile(filename, temp_path, 256);
	if (fd < 0)
		return (-1);
	if (write_all_lines(fd, lines) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	link_or_copy_temp(temp_path, filename);
	ft_strlcpy(temp_path, temp_path, 256);
	free_tab(lines);
	return (0);
}

int	tab_to_file_with_path(char **lines, const char *filename,
		char *temp_path_out, size_t path_size)
{
	char	*temp_path;

	temp_path = malloc(256);
	if (!temp_path)
		return (-1);
	if (write_to_temp_file(lines, filename, temp_path) < 0)
	{
		free(temp_path);
		return (-1);
	}
	ft_strlcpy(temp_path_out, temp_path, path_size);
	free(temp_path);
	return (0);
}

int	is_limiter_line(char *line, char *limiter)
{
	return (ft_strcmp(line, limiter) == 0);
}
