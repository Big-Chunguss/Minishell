/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_command_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:00:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 12:56:00 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	setup_command_environment(t_cmd_params *params)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	redirect_io2(params->input_fd, params->output_fd);
}

int	prepare_command_args(t_ast *node, t_cleanup_params *cleanup, char ***tab,
		char **path)
{
	*tab = build_argv(node);
	cleanup->tab = *tab;
	cleanup->path = NULL;
	if (!*tab)
		cleanup_and_exit(1, cleanup);
	if (apply_redirections2(node) == -1)
		cleanup_and_exit(1, cleanup);
	*path = get_cmd_path((**tab), cleanup->env);
	cleanup->path = *path;
	return (0);
}

void	handle_empty_cmd_fork(t_ast *node, t_ast **head, t_ast **env,
		t_cmd_params *params)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		run_command(node, head, env, params);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		(*env)->env->error_code = WEXITSTATUS(status);
}
