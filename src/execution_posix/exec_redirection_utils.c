/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 15:35:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 11:49:14 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	build_prefix(const char *limiter, char *prefix, size_t size)
{
	size_t		idx;
	size_t		j;
	const char	*base;

	while (*limiter == ' ' || *limiter == '\t')
		limiter++;
	idx = 0;
	base = "minishell_heredoc_";
	while (base[idx] && idx < size - 1)
	{
		prefix[idx] = base[idx];
		idx++;
	}
	j = 0;
	while (limiter[j] && idx < size - 2)
		prefix[idx++] = limiter[j++];
	if (idx < size - 1)
		prefix[idx++] = '_';
	prefix[idx] = 0;
}

static void	build_fullpath(const char *name, char *full, size_t size)
{
	size_t		idx;
	size_t		j;
	const char	*tmpdir;

	idx = 0;
	tmpdir = "/tmp/";
	while (tmpdir[idx] && idx < size - 1)
	{
		full[idx] = tmpdir[idx];
		idx++;
	}
	j = 0;
	while (name[j] && idx < size - 1)
		full[idx++] = name[j++];
	full[idx] = 0;
}

char	*find_heredoc_file(const char *limiter)
{
	DIR				*dir;
	struct dirent	*ent;
	char			prefix[256];
	char			full[512];
	char			best[512];
	struct stat		st;

	build_prefix(limiter, prefix, sizeof(prefix));
	best[0] = 0;
	dir = opendir("/tmp");
	if (!dir)
		return (NULL);
	while ((ent = readdir(dir)) != NULL)
	{
		if (ft_strncmp(ent->d_name, prefix, ft_strlen(prefix)) == 0)
		{
			build_fullpath(ent->d_name, full, sizeof(full));
			if (stat(full, &st) == 0)
				ft_strlcpy(best, full, sizeof(best));
		}
	}
	closedir(dir);
	if (best[0] == 0)
		return (NULL);
	return (ft_strdup(best));
}
