/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:25:03 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 22:54:20 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*heredoc_readline(void)
{
	char	*line;

	write(1, "> ", 2);
	line = get_next_line(0);
	if (!line)
		return (NULL);
	line[ft_strcspn(line, "\n")] = '\0';
	return (line);
}

void	read_heredoc(char *limiter, int quoted_limiter, t_ast **env)
{
	char	*line;
	char	*str;
	char	*temp_filename;

	str = ft_strdup("");
	if (!str)
		return ;
	temp_filename = ft_strdup(limiter);
	while (1)
	{
		line = heredoc_readline();
		if (!line)
			break ;
		if (is_limiter_line(line, limiter))
		{
			free(line);
			break ;
		}
		maybe_expand_line(&line, quoted_limiter, env);
		append_line(&str, line);
		free(line);
	}
	finalize_heredoc(str, temp_filename);
}

void	read_heredoc_consume_only(char *limiter)
{
	char	*line;

	while (1)
	{
		line = heredoc_readline();
		if (!line)
			break ;
		if (is_limiter_line(line, limiter))
		{
			free(line);
			break ;
		}
		free(line);
	}
}
