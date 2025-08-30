/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file_writer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/29 18:17:22 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/// @brief Writes an array of strings to a file
/// @param lines NULL-terminated array of strings to write
/// @param filename Path of the file to write to
void	tab_to_file(char **lines, const char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		return ;
	}
	write_all_lines(fd, lines);
	close(fd);
}

/// @brief Checks if a line matches the heredoc limiter
/// @param line Input line to check
/// @param limiter Heredoc limiter string to match against
/// @return 1 if line matches limiter exactly, 0 otherwise
int	is_limiter_line(char *line, char *limiter)
{
	return (ft_strcmp(line, limiter) == 0);
}

/// @brief Generates a random temporary filename for heredoc
/// @param file_name Buffer to store the generated filename (must be at least 256 bytes)
void	get_random_filename(char *file_name)
{
	size_t			i;
	char			*charset;
	unsigned char	buffer[44];
	int				fd;

	i = 13;
	charset = "abcdefghijklmnopqrstuvwxyz0123456789";
	ft_strlcpy(file_name, "/tmp/heredoc_", 14);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return ;
	if (read(fd, buffer, 44) < 0)
	{
		close(fd);
		return ;
	}
	while (i < 44)
	{
		file_name[i] = charset[buffer[i] % 36];
		i++;
	}
	file_name[i] = 0;
	close(fd);
}
