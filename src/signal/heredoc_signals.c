/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 11:30:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 19:42:35 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	handle_sigint_heredoc(int sig_num)
{
	write(1, "\n", 1);
	g_exit_code = 130;
	(void)sig_num;
}

void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_parent_signals(void)
{
	setup_sigint_handler();
	setup_sigquit_handler();
}

void	handle_child_sigquit(int sig)
{
	(void)sig;
	g_exit_code = 131;
	signal(SIGQUIT, SIG_DFL);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, handle_child_sigquit);
}



void	ignore_signals_during_execution(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
