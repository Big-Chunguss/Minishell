/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 15:45:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/30 17:13:08 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
