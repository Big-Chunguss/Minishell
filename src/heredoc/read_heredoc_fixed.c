/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc_fixed.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:25:03 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 12:01:45 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**split_by_newline(char *str)
{
	char	**result;
	int		line_count;
	int		len;
	int		i;

	line_count = 1;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			line_count++;
		i++;
	}
	return (split_newline_alloc(str, line_count));
}

char	*heredoc_readline(void)
{
	char	*line;

	write(1, "> ", 2);
	line = get_next_line(0);
	if (!line)
		return (NULL);
	line[strcspn(line, "\n")] = '\0';
	return (line);
}

void	read_heredoc(char *limiter, int quoted_limiter, t_ast **env)
{
	char	*str;
	char	*temp_filename;
	char	**res;

	str = ft_strdup("");
	if (!str)
		return ;
	temp_filename = ft_strdup(limiter);
	heredoc_process_lines(&str, limiter, quoted_limiter, env);
	if (g_exit_code != 130)
	{
		res = split_by_newline(str);
		free(str);
		tab_to_file(res, temp_filename);
		free(temp_filename);
	}
	else
	{
		free(str);
		free(temp_filename);
	}
}
