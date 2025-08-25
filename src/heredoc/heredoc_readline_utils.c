/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_readline_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:35:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 12:56:00 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	maybe_expand_line(char **line, int quoted_limiter, t_ast **env)
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

void	append_line(char **acc, char *line)
{
	char	*tmp;

	tmp = ft_strjoin(*acc, line);
	free(*acc);
	*acc = tmp;
	tmp = ft_strjoin(*acc, "\n");
	free(*acc);
	*acc = tmp;
}

void	finalize_heredoc(char *str, char *filename)
{
	extern int	g_exit_code;
	char		**res;

	if (g_exit_code != 130)
	{
		res = split_by_newline(str);
		free(str);
		tab_to_file(res, filename);
		free(filename);
	}
	else
	{
		free(str);
		free(filename);
	}
}
