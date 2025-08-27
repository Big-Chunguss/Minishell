/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 17:23:39 by hfragnol          #+#    #+#             */
/*   Updated: 2025/08/27 23:48:46 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_i_and_j(int *i, int *j)
{
	*j = 0;
	*i = -1;
}

bool	return_quotes_error(char *line)
{
	int	total_count;
	int	count_single;
	int	count_double;

	count_single = count_sing_quotes(line);
	count_double = count_double_quotes(line);
	total_count = count_total_quotes(line);
	if (total_count % 2 != 0 && total_count != 0)
	{
		if (count_single % 2 != 0)
		{
			ft_putstr_fd("syntax error: unclosed single quote\n",
				STDERR_FILENO);
			return (false);
		}
		if (count_double % 2 != 0)
		{
			ft_putstr_fd("syntax error: unclosed double quote\n",
				STDERR_FILENO);
			return (false);
		}
	}
	return (true);
}

static bool	redir_sequence_error(char *line, int i, t_ast **env)
{
	int	count;

	count = i + 1;
	while (line[count] == ' ' || line[count] == '\t')
		count++;
	if ((line[count] == '>' || line[count] == '<') && count != i + 1)
	{
		ft_putstr_fd("syntax error near unexpected token 'redir \n",
			STDERR_FILENO);
		(*env)->env->error_code = 2;
		return (true);
	}
	return (false);
}

bool	too_much_redir(char *line, t_ast **env)
{
	int			i;
	int			count;
	t_qtstate	state;

	init_qt_state(&state);
	init_i_and_j(&i, &count);
	while (line[++i])
	{
		update_quotes_state(&state, line[i]);
		if (!state.single_open && !state.double_open)
		{
			if ((line[i] == '>' || line[i] == '<') && line[i + 1]
				&& line[i + 1] != line[i]
				&& redir_sequence_error(line, i, env))
				return (true);
		}
	}
	return (false);
}
