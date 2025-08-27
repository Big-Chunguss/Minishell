/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_hugo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfragnol <hfragnol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 17:37:53 by hfragnol          #+#    #+#             */
/*   Updated: 2025/08/26 17:38:12 by hfragnol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_sing_quotes(const char *line)
{
	int		i;
	int		count;
	bool	in_single;
	bool	in_double;

	i = 0;
	count = 0;
	in_single = false;
	in_double = false;
	while (line[i])
	{
		if (line[i] == '\'' && !in_double)
		{
			in_single = !in_single;
			count++;
		}
		else if (line[i] == '"' && !in_single)
		{
			in_double = !in_double;
		}
		i++;
	}
	return (count);
}

int	count_double_quotes(const char *line)
{
	int		i;
	int		count;
	bool	in_single;
	bool	in_double;

	i = 0;
	count = 0;
	in_single = false;
	in_double = false;
	while (line[i])
	{
		if (line[i] == '"' && !in_single)
		{
			in_double = !in_double;
			count++;
		}
		else if (line[i] == '\'' && !in_double)
		{
			in_single = !in_single;
		}
		i++;
	}
	return (count);
}

int	count_total_quotes(char *line)
{
	int	total_count;

	total_count = count_double_quotes(line) + count_sing_quotes(line);
	return (total_count);
}

void	init_qt_state(t_qtstate *state)
{
	if (!state)
		return ;
	state->single_open = false;
	state->double_open = false;
}

void	update_quotes_state(t_qtstate *state, char c)
{
	if (c == '\'' && state->double_open == false)
		state->single_open = !state->single_open;
	else if (c == '\"' && state->single_open == false)
		state->double_open = !state->double_open;
}
