/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_path_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 13:43:31 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	build_prefix(const char *limiter, char *prefix, size_t size)
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

void	build_fullpath(const char *name, char *full, size_t size)
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
