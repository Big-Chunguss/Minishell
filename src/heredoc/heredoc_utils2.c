/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:41:09 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 12:37:31 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	tab_to_file(char **lines, const char *filename)
{
	int		fd;
	char	temp_path[256];

	fd = open_tempfile(filename, temp_path, sizeof(temp_path));
	if (fd < 0)
		return (free_tab(lines));
	if (write_all_lines(fd, lines) < 0)
	{
		close(fd);
		return (free_tab(lines));
	}
	close(fd);
	link_or_copy_temp(temp_path, filename);
	free_tab(lines);
}

int	tab_to_file_with_path(char **lines, const char *filename, char *temp_path_out, size_t path_size)
{
	int		fd;
	char	temp_path[256];

	fd = open_tempfile(filename, temp_path, sizeof(temp_path));
	if (fd < 0)
	{
		free_tab(lines);
		return (-1);
	}
	if (write_all_lines(fd, lines) < 0)
	{
		close(fd);
		free_tab(lines);
		return (-1);
	}
	close(fd);
	link_or_copy_temp(temp_path, filename);
	// Copy the temp path to the output buffer
	ft_strlcpy(temp_path_out, temp_path, path_size);
	free_tab(lines);
	return (0);
}

void	clean_heredoc(char **argv)
{
	char	temp_path[256];
	char	pid_str[16];
	int		i;
	int		pid;

	if (!ft_strcmp(argv[1], "here_doc"))
		free(argv[1]);
	get_next_line(-1);
	pid = getpid();
	i = 0;
	if (pid == 0)
		pid_str[i++] = '0';
	else
	{
		while (pid > 0)
		{
			pid_str[i++] = (pid % 10) + '0';
			pid /= 10;
		}
	}
	pid_str[i] = '\0';
	ft_strlcpy(temp_path, "/tmp/minishell_heredoc_*_", sizeof(temp_path));
	ft_strlcat(temp_path, pid_str, sizeof(temp_path));
}

void	check_heredoc(t_token **lst, t_ast **env)
{
	t_token		*list;
	t_token		*heredoc_limiters[100];  // Array to store heredoc delimiters
	int			heredoc_count;
	extern int	g_exit_code;
	int			saved_exit_code;
	int			i;

	saved_exit_code = g_exit_code;
	list = *lst;
	heredoc_count = 0;
	
	// Collect all heredoc delimiters in order
	while (list && heredoc_count < 100)
	{
		if (list->type == HEREDOC)
		{
			if (list->next && list->next->value)
			{
				heredoc_limiters[heredoc_count] = list->next;
				heredoc_count++;
			}
		}
		list = list->next;
	}
	
	// Process all heredocs: consume all but the last one
	i = 0;
	while (i < heredoc_count)
	{
		if (i == heredoc_count - 1)
		{
			// Last heredoc - save its content
			start_heredoc(heredoc_limiters[i]->value, heredoc_limiters[i]->was_quoted, env);
		}
		else
		{
			// Earlier heredocs - just consume without saving
			read_heredoc_consume_only(heredoc_limiters[i]->value);
		}
		if (g_exit_code == 130)
			break ;
		i++;
	}
	
	if (g_exit_code != 130)
		g_exit_code = saved_exit_code;
}

int	is_limiter_line(char *line, char *limiter)
{
	return (ft_strcmp(line, limiter) == 0);
}
