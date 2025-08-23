/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 16:16:42 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 13:10:07 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>
#include <unistd.h>


char	*ft_replace(char *buffer)

{
	int		i;
	int		j;
	char	*tmp;
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	tmp = ft_calloc(sizeof(char), ft_strlen(buffer) - i + 1);
	if (!tmp)
		return (NULL);
	j = 0;
	while (buffer[i])
		tmp[j++] = buffer[i++];
	free(buffer);
	return (tmp);
}
char	*ft_extract(char *buffer)

{
	int		i;
	char	*line;
	i = 0;
	if (!buffer || !buffer[i])
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	line = ft_calloc(sizeof(char), i + 2);
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		line[i] = '\n';
	return (line);
}
char	*ft_buffer(int fd, char *buffer)
{
	int	should_break;

	if (!buffer)
		buffer = ft_calloc(1, 1);
	if (!buffer)
		return (NULL);
	while (1)
	{
		buffer = read_and_join(buffer, fd, &should_break);
		if (!buffer || should_break)
			break ;
	}
	return (buffer);
}
char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (!validate_fd_and_cleanup(fd, &buffer))
		return (NULL);
	buffer = ft_buffer(fd, buffer);
	if (!buffer)
		return (NULL);
	line = ft_extract(buffer);
	buffer = ft_replace(buffer);
	if (!line && buffer)
		cleanup_buffer(&buffer);
	return (line);
}
void	cleanup_get_next_line(void)

{
	get_next_line(-42);
}
