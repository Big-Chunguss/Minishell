/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 15:45:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 12:01:45 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"

static void	maybe_expand_line(char **line, int quoted_limiter, t_ast **env)

{
	char	*expanded;
	if (quoted_limiter)
		return ;
	expanded = unquoted_var_expansion(*line, env);
	if (expanded != *line)
	{
		free(*line);
		*line = expanded;
	}
}
static void	append_line(char **acc, char *line)

{
	char	*tmp;
	tmp = ft_strjoin(*acc, line);
	free(*acc);
	*acc = tmp;
	tmp = ft_strjoin(*acc, "\n");
	free(*acc);
	*acc = tmp;
}
void	heredoc_process_lines(char **str, char *limiter, int quoted_limiter,
		t_ast **env)

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
		maybe_expand_line(&line, quoted_limiter, env);
		append_line(str, line);
		free(line);
	}
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
