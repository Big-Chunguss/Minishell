/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:30:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 12:56:16 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup_buffer(char **buffer)
{
	if (*buffer)
	{
		free(*buffer);
		*buffer = NULL;
	}
}

int	validate_fd_and_cleanup(int fd, char **buffer)
{
	if (fd == -42)
	{
		cleanup_buffer(buffer);
		return (0);
	}
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		cleanup_buffer(buffer);
		return (0);
	}
	return (1);
}

char	*read_and_join(char *buffer, int fd, int *should_break)
{
	char	*tmp;
	char	*new_buffer;
	int		bytes;

	tmp = ft_calloc(sizeof(char), BUFFER_SIZE + 1);
	if (!tmp)
		return (free(buffer), NULL);
	bytes = read(fd, tmp, BUFFER_SIZE);
	if (bytes < 0)
	{
		free(tmp);
		free(buffer);
		return (NULL);
	}
	new_buffer = ft_strjoin(buffer, tmp);
	free(buffer);
	free(tmp);
	if (!new_buffer)
		return (NULL);
	*should_break = (bytes == 0 || ft_newline(new_buffer));
	return (new_buffer);
}
