/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:33:49 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 12:37:31 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"

extern int	g_exit_code;
static int	print_redir_error(const char *path)

{
	if (errno == EACCES)
	{
		write(2, path, ft_strlen(path));
		write(2, ": Permission denied\n", 20);
	}
	else if (errno == ENOENT)
	{
		write(2, path, ft_strlen(path));
		write(2, ": No such file or directory\n", 28);
	}
	else
	{
		write(2, path, ft_strlen(path));
		write(2, ": ", 2);
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
	}
	return (-1);
}
static int	open_output_redir(t_ast *redir)

{
	int	fd;
	if (!ft_strcmp(redir->value, ">"))
		fd = open(redir->target->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->target->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (print_redir_error(redir->target->value));
	return (fd);
}
static int	open_input_redir(t_ast *redir)

{
	int	fd;
	fd = open(redir->target->value, O_RDONLY);
	if (fd < 0)
		return (print_redir_error(redir->target->value));
	return (fd);
}
static int	handle_input_redirection(t_ast *child)

{
	int	fd;
	fd = open_input_redir(child);
	if (fd == -1)
		return (-1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}
static int	handle_output_redirection(t_ast *child)

{
	int	fd;
	fd = open_output_redir(child);
	if (fd == -1)
		return (-1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}
static char	*find_heredoc_file(const char *limiter)

{
	char			pattern[256];
	char			*result;
	DIR				*dir;
	struct dirent	*entry;
	const char		*trimmed_limiter;
	char			prefix[256];
	char			best_file[256];
	struct stat		st;
	struct stat		best_stat;
	int				found;
	trimmed_limiter = limiter;
	while (*trimmed_limiter && (*trimmed_limiter == ' ' || *trimmed_limiter == '\t'))
		trimmed_limiter++;
	ft_strlcpy(prefix, "minishell_heredoc_", sizeof(prefix));
	ft_strlcat(prefix, trimmed_limiter, sizeof(prefix));
	ft_strlcat(prefix, "_", sizeof(prefix));
	dir = opendir("/tmp");
	if (!dir)
		return (NULL);
	found = 0;
	best_stat.st_mtime = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		if (ft_strncmp(entry->d_name, prefix, ft_strlen(prefix)) == 0)
		{
			ft_strlcpy(pattern, "/tmp/", sizeof(pattern));
			ft_strlcat(pattern, entry->d_name, sizeof(pattern));
			if (stat(pattern, &st) == 0)
			{
				if (!found || st.st_mtime > best_stat.st_mtime)
				{
					best_stat = st;
					ft_strlcpy(best_file, pattern, sizeof(best_file));
					found = 1;
				}
			}
		}
	}
	closedir(dir);
	if (found)
		result = ft_strdup(best_file);
	else
		result = NULL;
	return (result);
}
int	process_redirection_child(t_ast *child)

{
	char	*heredoc_path;
	if (g_exit_code == 130)
		return (-1);
	if (!ft_strcmp(child->value, "<"))
		return (handle_input_redirection(child));
	else if (!ft_strcmp(child->value, "<<"))
	{
		heredoc_path = find_heredoc_file(child->target->value);
		if (!heredoc_path)
		{
			write(2, "No heredoc file found!\n", 23);
			return (-1);
		}
		free(child->target->value);
		child->target->value = heredoc_path;
		return (handle_input_redirection(child));
	}
	else if (!ft_strcmp(child->value, ">") || !ft_strcmp(child->value, ">>"))
		return (handle_output_redirection(child));
	return (0);
}
