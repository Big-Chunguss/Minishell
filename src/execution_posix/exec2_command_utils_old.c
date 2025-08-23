/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_command_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:00:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 16:32:00 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	setup_command_environment(t_cmd_params *params)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	redirect_io2(params->input_fd, params->output_fd);
}

int	prepare_command_args(t_ast *node, t_cleanup_params *cleanup,
		char ***tab, char **path)
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

static void	handle_redirection_only(t_ast *node, t_ast **head, t_ast **env,
		t_cmd_params *params)
{
	t_cleanup_params	redir_cleanup;

	redir_cleanup.head = head;
	redir_cleanup.env = env;
	redir_cleanup.tab = NULL;
	redir_cleanup.path = NULL;
	setup_command_environment(params);
	if (node && apply_redirections2(node) == -1)
		cleanup_and_exit(1, &redir_cleanup);
	cleanup_and_exit(0, &redir_cleanup);
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

void	run_command(t_ast *node, t_ast **head, t_ast **env,
		t_cmd_params *params)
{
	char				**tab;
	char				*path;
	int					is_external;
	t_cleanup_params	cleanup;

	if (!node || !node->value || ft_strlen(node->value) == 0)
	{
		handle_redirection_only(node, head, env, params);
		return ;
	}
	tab = NULL;
	path = NULL;
	cleanup.head = head;
	cleanup.env = env;
	setup_command_environment(params);
	prepare_command_args(node, &cleanup, &tab, &path);
	is_external = cmd(tab, path, env);
	if (!is_external)
	{
		cleanup.tab = NULL;
		cleanup.path = NULL;
		cleanup_and_exit((*env)->env->error_code, &cleanup);
	}
	handle_external_command(tab, path, env, head);
}
