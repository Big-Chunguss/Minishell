/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file_finder.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 12:30:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/29 17:50:59 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*char	*find_heredoc_file(void)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*result;

	dir = opendir("/tmp");
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (ft_strncmp(entry->d_name, "heredoc_", 8) == 0)
		{
			result = malloc(256);
			if (!result)
			{
				closedir(dir);
				return (NULL);
			}
			ft_strlcpy(result, "/tmp/", 256);
			ft_strlcat(result, entry->d_name, 256);
			return (closedir(dir), result);
		}
		entry = readdir(dir);
	}
	return (closedir(dir), NULL);
}
*/
char	*find_heredoc_file(void)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*result;

	dir = opendir("/tmp");
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (ft_strncmp(entry->d_name, "heredoc_", 8) == 0)
		{
			result = malloc(256);
			if (!result)
			{
				closedir(dir);
				return (NULL);
			}
			ft_strlcpy(result, "/tmp/", 256);
			ft_strlcat(result, entry->d_name, 256);
			closedir(dir);
			return (result);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (NULL);
}
