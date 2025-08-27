/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_command_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:00:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 23:21:37 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

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

void	handle_external_command(char **tab, char *path, t_ast **env,
		t_ast **head)
{
	t_cleanup_params	cleanup;

	cleanup.head = head;
	cleanup.env = env;
	cleanup.tab = tab;
	cleanup.path = path;
	if (path == NULL || access(path, F_OK) < 0)
	{
		command_not_found_error(tab[0]);
		(*env)->env->error_code = 127;
		cleanup_and_exit((*env)->env->error_code, &cleanup);
	}
	else if (is_directory(path) || access(path, X_OK) != 0)
	{
		(*env)->env->error_code = 126;
		cleanup_and_exit((*env)->env->error_code, &cleanup);
	}
	execve(path, tab, (*env)->env->env);
	(*env)->env->error_code = 1;
	cleanup_and_exit((*env)->env->error_code, &cleanup);
}
