/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:30:17 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/29 18:17:22 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/// @brief Starts heredoc processing and returns the temporary file path
/// @param limiter String that marks the end of heredoc input
/// @param quoted_limiter Flag indicating if limiter was quoted (affects expansion)
/// @param env Pointer to environment AST for variable expansion
/// @return String containing path to temporary heredoc file, or NULL on error
char	*start_heredoc(char *limiter, int quoted_limiter, t_ast **env)
{
	char	heredoc_path[256];

	read_heredoc(limiter, quoted_limiter, env, heredoc_path);
	if (heredoc_path[0] != '\0')
		return (ft_strdup(heredoc_path));
	return (NULL);
}

/// @brief Copies content from temporary file to target file
/// @param temp_path Path to the source temporary file
/// @param filename Path to the destination file
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

int	open_tempfile(char *temp_path)
{
	int	fd;

	get_random_filename(temp_path);
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
	copy_tmp_to_file(temp_path, filename);
}
