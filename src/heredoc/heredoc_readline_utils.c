/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_readline_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:35:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/29 18:01:35 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	maybe_expand_line(char **line, int quoted_limiter, t_ast **env, char *output_path)
{
	char	*expanded;

	(void)output_path;

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

void	process_heredoc_content(char *str, char *final_path)
{
	char	**res;

	res = split_by_newline(str);
	tab_to_file(res, final_path);
	free_tab(res);
}

void	finalize_heredoc(char *str, char *temp_filename)
{
	extern int	g_exit_code;

	if (g_exit_code != 130)
	{
		process_heredoc_content(str, temp_filename);
	}
	else
	{
		free(str);
	}
}
