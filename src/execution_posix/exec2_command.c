/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:10:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/26 15:29:49 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

extern int	g_exit_code;

static void	handle_external_command(char **tab, char *path, t_ast **env,
		t_ast **head)
{
	int					code;
	t_cleanup_params	cleanup;
	int					has_slash;

	has_slash = (strchr(tab[0], '/') != NULL);
	cleanup.head = head;
	cleanup.env = env;
	cleanup.tab = tab;
	cleanup.path = path;
	code = classify_error(tab[0], path);
	if (code != 0)
	{
		print_exec_error(tab[0], code, has_slash);
		(*env)->env->error_code = code;
		cleanup_and_exit(code, &cleanup);
	}
	execve(path, tab, (*env)->env->env);
	if (errno == EACCES)
		(*env)->env->error_code = 126;
	else if (errno == ENOENT)
		(*env)->env->error_code = 127;
	else
		(*env)->env->error_code = 1;
	print_exec_error(tab[0], (*env)->env->error_code, has_slash);
	cleanup_and_exit((*env)->env->error_code, &cleanup);
}

static void	handle_empty_command(t_ast *node, t_ast **head, t_ast **env,
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

void	run_command(t_ast *node, t_ast **head, t_ast **env,
		t_cmd_params *params)
{
	char				**tab;
	char				*path;
	int					is_external;
	t_cleanup_params	cleanup;

	if (!node || !node->value || ft_strlen(node->value) == 0)
	{
		handle_empty_command(node, head, env, params);
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

void	execute_external_command(t_ast *node, t_ast **head, t_ast **env,
		t_cmd_params *params)
{
	pid_t	pid;
	int		status;

	ignore_signals_during_execution();
	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		run_command(node, head, env, params);
	}
	status = 0;
	waitpid(pid, &status, 0);
	restore_parent_signals();
	if (WIFEXITED(status))
		(*env)->env->error_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		(*env)->env->error_code = 128 + WTERMSIG(status);
}

void	execute_builtin_command(t_ast *node, t_ast **head, t_ast **env,
		t_cmd_params *params)
{
	char				**tab;
	char				*path;
	t_cleanup_params	cleanup;

	(void)head;
	tab = NULL;
	path = NULL;
	cleanup.head = head;
	cleanup.env = env;
	setup_command_environment(params);
	prepare_command_args(node, &cleanup, &tab, &path);
	build_in(tab, 0, env);
	if (tab)
		free_argv_shallow(tab);
	if (path)
		free(path);
}
