/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:30:17 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 19:42:31 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	start_heredoc(char *limiter, int quoted_limiter, t_ast **env)
{
	read_heredoc(limiter, quoted_limiter, env);
}

void	copy_tmp_to_file(const char *temp_path, const char *filename)
{
	int		fd;
	int		src_fd;
	char	*buffer;
	ssize_t	bytes_read;

	buffer = malloc(4096);
	if (!buffer)
		return ;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd >= 0)
	{
		src_fd = open(temp_path, O_RDONLY);
		if (src_fd >= 0)
		{
			bytes_read = read(src_fd, buffer, 4096);
			while (bytes_read > 0)
			{
				write(fd, buffer, bytes_read);
				bytes_read = read(src_fd, buffer, 4096);
			}
			close(src_fd);
		}
		close(fd);
	}
	free(buffer);
}

int	open_tempfile(const char *filename, char *temp_path, size_t path_size)
{
	int	fd;

	build_temp_path(temp_path, path_size, filename, getpid());
	fd = open(temp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		write(2, "open: ", 6);
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
	}
	return (fd);
}

int	write_all_lines(int fd, char **lines)
{
	int		i;
	ssize_t	written;

	i = 0;
	while (lines && lines[i])
	{
		written = write(fd, lines[i], ft_strlen(lines[i]));
		if (written < 0)
		{
			write(2, "write: ", 7);
			write(2, strerror(errno), ft_strlen(strerror(errno)));
			write(2, "\n", 1);
			return (-1);
		}
		write(fd, "\n", 1);
		i++;
	}
	return (0);
}

void	link_or_copy_temp(const char *temp_path, const char *filename)
{
	unlink(filename);
	if (symlink(temp_path, filename) < 0)
		copy_tmp_to_file(temp_path, filename);
}
