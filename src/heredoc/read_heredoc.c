/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:25:03 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/29 18:17:22 by agaroux          ###   ########.fr       */
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

/// @brief Main heredoc reading function that processes user input until limiter
/// @param limiter String that marks the end of heredoc input
/// @param quoted_limiter Flag indicating if limiter was quoted (affects variable expansion)
/// @param env Pointer to environment AST for variable expansion
/// @param output_path Buffer to store the path of generated temporary file
void	read_heredoc(char *limiter, int quoted_limiter, t_ast **env,
		char *output_path)
{
	char	*line;
	char	*str;
	char	temp_path[256];

	str = ft_strdup("");
	if (!str)
		return ;
	get_random_filename(temp_path);
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
		maybe_expand_line(&line, quoted_limiter, env, output_path);
		append_line(&str, line);
		free(line);
	}
	finalize_heredoc(str, temp_path);
	ft_strlcpy(output_path, temp_path, 256);
	free(str);
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
