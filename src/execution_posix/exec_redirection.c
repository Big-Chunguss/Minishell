/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:33:49 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 15:05:00 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

static int	print_redir_error(const char *path)
{
	if (errno == EACCES)
		fprintf(stderr, "%s: Permission denied\n", path);
	else if (errno == ENOENT)
		fprintf(stderr, "%s: No such file or directory\n", path);
	else
		perror(path);
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

/// @brief checking if node contains an input redirection and applying
/// it to STDIN
/// @param node
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
	char		pattern[256];
	char		*result;
	char		command[512];
	FILE		*fp;
	char		line[256];
	int			len;
	const char	*trimmed_limiter;

	// Skip leading whitespace in limiter
	trimmed_limiter = limiter;
	while (*trimmed_limiter && (*trimmed_limiter == ' ' || *trimmed_limiter == '\t'))
		trimmed_limiter++;

	// Create a pattern to match heredoc files for this limiter, sorted by time (most recent first)
	snprintf(pattern, sizeof(pattern), "/tmp/minishell_heredoc_%s_*", trimmed_limiter);
	snprintf(command, sizeof(command), "ls -t %s 2>/dev/null | head -1", pattern);
	
	fp = popen(command, "r");
	if (!fp)
		return (NULL);
	
	if (fgets(line, sizeof(line), fp) != NULL)
	{
		// Remove newline if present
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		result = ft_strdup(line);
	}
	else
		result = NULL;
	
	pclose(fp);
	return (result);
}

int	process_redirection_child(t_ast *child)
{
	char	*heredoc_path;

	if (g_exit_code == 130)
		return (-1);
	if (!strcmp(child->value, "<"))
		return (handle_input_redirection(child));
	else if (!strcmp(child->value, "<<"))
	{
		// For heredoc, find the temp file
		heredoc_path = find_heredoc_file(child->target->value);
		if (!heredoc_path)
		{
			write(2, "No heredoc file found!\n", 23);
			return (-1);
		}
		// Update the target to point to the temp file
		free(child->target->value);
		child->target->value = heredoc_path;
		return (handle_input_redirection(child));
	}
	else if (!strcmp(child->value, ">") || !strcmp(child->value, ">>"))
		return (handle_output_redirection(child));
	return (0);
}
