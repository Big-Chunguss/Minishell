/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_command_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:00:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 23:22:13 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
