/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfragnol <hfragnol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 17:23:39 by hfragnol          #+#    #+#             */
/*   Updated: 2025/08/26 17:37:43 by hfragnol         ###   ########.fr       */
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
			ft_putstr_fd("syntax error: unclosed single quote\n", STDERR_FILENO);
			return (false);
		}
		if (count_double % 2 != 0)
		{
			ft_putstr_fd("syntax error: unclosed double quote\n", STDERR_FILENO);
			return (false);
		}
	}
	return (true);
}

bool	too_much_redir(char *line)
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
			if ((line[i] == '>' || line[i] == '<') && (line[i + 1] != line[i]
					&& line[i + 1]))
			{
				count = i + 1;
				while (line[count] == ' ' || line[count] == '\t')
					count++;
				if ((line[count] == '>' || line[count] == '<') && (count != i
						+ 1))
					return (ft_putstr_fd("syntax error near unexpected token"
							" 'redir \n", STDERR_FILENO), true);
			}
		}
	}
	return (false);
}
