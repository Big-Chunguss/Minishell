/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:25:03 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/22 19:48:33 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char **split_by_newline(char *str)
{
	char	**result;
	int		line_count;
	int		i, j, start;
	int		len;

	// Count lines
	line_count = 1;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			line_count++;
		i++;
	}
	
	// Allocate result array
	result = malloc(sizeof(char *) * (line_count + 1));
	if (!result)
		return (NULL);
	
	// Split by newlines
	i = 0;
	j = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == '\n')
		{
			len = i - start;
			result[j] = malloc(len + 1);
			if (!result[j])
			{
				// Free allocated memory on failure
				while (--j >= 0)
					free(result[j]);
				free(result);
				return (NULL);
			}
			ft_strlcpy(result[j], str + start, len + 1);
			j++;
			start = i + 1;
		}
		i++;
	}
	
	// Handle last line if string doesn't end with newline
	if (start < i)
	{
		len = i - start;
		result[j] = malloc(len + 1);
		if (!result[j])
		{
			// Free allocated memory on failure
			while (--j >= 0)
				free(result[j]);
			free(result);
			return (NULL);
		}
		ft_strlcpy(result[j], str + start, len + 1);
		j++;
	}
	
	result[j] = NULL;
	return (result);
}

static char	*heredoc_readline(void)
{
	char	*line;

	write(1, "> ", 2);
	line = get_next_line(0);
	if (!line)
		return (NULL);
	line[strcspn(line, "\n")] = '\0';
	return (line);
}

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

static void	finalize_heredoc(char *str, char *filename)
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
