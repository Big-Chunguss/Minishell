/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file_finder.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 12:30:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 22:32:30 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_and_copy_match(const char *name, const char *prefix,
		char *best)
{
	char		*full;
	struct stat	st;
	int			result;

	full = malloc(512);
	if (!full)
		return (0);
	if (ft_strncmp(name, prefix, ft_strlen(prefix)) != 0)
	{
		free(full);
		return (0);
	}
	build_fullpath(name, full, 512);
	if (stat(full, &st) != 0)
	{
		free(full);
		return (0);
	}
	ft_strlcpy(best, full, 512);
	result = 1;
	free(full);
	return (result);
}

static int	allocate_buffers(char **prefix, char **best)
{
	*prefix = malloc(256);
	if (!*prefix)
		return (0);
	*best = malloc(512);
	if (!*best)
	{
		free(*prefix);
		return (0);
	}
	return (1);
}

static void	cleanup_buffers(char *prefix, char *best)
{
	free(prefix);
	free(best);
}

static char	*process_directory(char *prefix, char *best)
{
	DIR				*dir;
	struct dirent	*ent;

	dir = opendir("/tmp");
	if (!dir)
		return (NULL);
	ent = readdir(dir);
	while (ent != NULL)
	{
		check_and_copy_match(ent->d_name, prefix, best);
		ent = readdir(dir);
	}
	closedir(dir);
	if (best[0] == 0)
		return (NULL);
	return (ft_strdup(best));
}

char	*find_heredoc_file(const char *limiter)
{
	char	*prefix;
	char	*best;
	char	*result;

	if (!allocate_buffers(&prefix, &best))
		return (NULL);
	build_prefix(limiter, prefix, 256);
	best[0] = 0;
	result = process_directory(prefix, best);
	cleanup_buffers(prefix, best);
	return (result);
}
