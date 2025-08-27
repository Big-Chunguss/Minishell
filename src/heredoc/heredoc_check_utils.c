/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_check_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:45:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 23:22:53 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	collect_heredoc_limiters(t_token *list, t_token **heredoc_limiters)
{
	int	heredoc_count;

	heredoc_count = 0;
	while (list && heredoc_count < 100)
	{
		if (list->type == HEREDOC)
		{
			if (list->next && list->next->value)
			{
				heredoc_limiters[heredoc_count] = list->next;
				heredoc_count++;
			}
		}
		list = list->next;
	}
	return (heredoc_count);
}

static void	process_heredoc_limiters(t_token **heredoc_limiters,
		int heredoc_count, t_ast **env)
{
	extern int	g_exit_code;
	int			i;

	i = 0;
	while (i < heredoc_count)
	{
		if (i == heredoc_count - 1)
		{
			start_heredoc(heredoc_limiters[i]->value,
				heredoc_limiters[i]->was_quoted, env);
		}
		else
		{
			read_heredoc_consume_only(heredoc_limiters[i]->value);
		}
		if (g_exit_code == 130)
			break ;
		i++;
	}
}

void	check_heredoc(t_token **lst, t_ast **env)
{
	t_token		*list;
	t_token		*heredoc_limiters[100];
	int			heredoc_count;
	extern int	g_exit_code;
	int			saved_exit_code;

	saved_exit_code = g_exit_code;
	list = *lst;
	heredoc_count = collect_heredoc_limiters(list, heredoc_limiters);
	if (heredoc_count != 0)
	{
		setup_heredoc_signals();
		process_heredoc_limiters(heredoc_limiters, heredoc_count, env);
		restore_parent_signals();
	}
	if (g_exit_code != 130)
		g_exit_code = saved_exit_code;
}
