/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_wait.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:10:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/22 19:48:33 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

extern int	g_exit_code;

void	wait_for_children(t_pipeline_data *data, pid_t last_pid, t_ast **env)
{
	int	status;
	int	last_status;
	int	i;

	last_status = 0;
	waitpid(last_pid, &last_status, 0);
	i = 0;
	while (i < data->idx)
	{
		if (data->children[i] != last_pid)
			waitpid(data->children[i], &status, 0);
		i++;
	}
	if (WIFEXITED(last_status))
		(*env)->env->error_code = WEXITSTATUS(last_status);
	else if (WIFSIGNALED(last_status))
	{
		g_exit_code = 128 + WTERMSIG(last_status);
		(*env)->env->error_code = g_exit_code;
	}
}

void	wait_for_all_pipeline_children(int cmd_count, pid_t last_pid, t_ast **env)
{
	int	status;
	int	last_status;
	int	i;

	last_status = 0;
	waitpid(last_pid, &last_status, 0);
	
	// Wait for all other children using waitpid(-1)
	i = 1; // We already waited for one (last_pid)
	while (i < cmd_count)
	{
		if (waitpid(-1, &status, 0) > 0)
			i++;
		else
			break;
	}
	
	if (WIFEXITED(last_status))
		(*env)->env->error_code = WEXITSTATUS(last_status);
	else if (WIFSIGNALED(last_status))
	{
		g_exit_code = 128 + WTERMSIG(last_status);
		(*env)->env->error_code = g_exit_code;
	}
}
