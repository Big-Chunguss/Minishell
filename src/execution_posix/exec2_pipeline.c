/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:10:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/22 15:31:43 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

extern int	g_exit_code;

static int	handle_pipe_creation(t_pipeline_data *data)
{
	if (pipe(data->pipefd) < 0)
	{
		if (data->children)
		{
			free(data->children);
			data->children = NULL;
		}
		return (-1);
	}
	return (0);
}

static void	handle_pipe_child(t_ast *cur, t_ast **head, t_ast **env,
		t_pipeline_data *data)
{
	t_cmd_params	params;

	close(data->pipefd[0]);
	params.input_fd = data->cur_in;
	params.output_fd = data->pipefd[1];
	run_command(cur->left, head, env, &params);
}

static void	handle_pipe_parent(t_pipeline_data *data, pid_t pid)
{
	// No longer store PID - we'll wait for any child with waitpid(-1)
	(void)pid; // Mark as unused to avoid compiler warning
	close(data->pipefd[1]);
	if (data->cur_in != STDIN_FILENO)
		close(data->cur_in);
	data->cur_in = data->pipefd[0];
	data->cur = data->cur->right;
}

void	run_pipeline_loop(t_ast **head, t_ast **env, t_pipeline_data *data)
{
	pid_t	pid;

	while (data->cur && data->cur->type == NODE_PIPE)
	{
		if (handle_pipe_creation(data) == -1)
			return ;
		pid = fork();
		if (pid == 0)
			handle_pipe_child(data->cur, head, env, data);
		handle_pipe_parent(data, pid);
	}
}
