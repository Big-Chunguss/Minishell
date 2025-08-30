/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:10:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/30 17:24:45 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

extern int	g_exit_code;

/// @brief Handles execution of empty commands with redirections only
/// @param node AST node (may be NULL for empty command)
/// @param head Pointer to AST head for context
/// @param env Pointer to environment AST
/// @param params Command execution parameters including file descriptors
static void	handle_empty_command(t_ast *node, t_ast **head, t_ast **env,
		t_cmd_params *params)
{
	t_cleanup_params	redir_cleanup;

	redir_cleanup.head = head;
	redir_cleanup.env = env;
	redir_cleanup.tab = NULL;
	redir_cleanup.path = NULL;
	redirect_io2(params->input_fd, params->output_fd);
	if (node && apply_redirections2(node) == -1)
		cleanup_and_exit(1, &redir_cleanup);
	cleanup_and_exit(0, &redir_cleanup);
}

/// @brief Main command execution function for individual commands
/// @param node AST node representing the command to execute
/// @param head Pointer to AST head for context
/// @param env Pointer to environment AST
/// @param params Command execution parameters including file descriptors
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
	redirect_io2(params->input_fd, params->output_fd);
	prepare_command_args(node, &cleanup, &tab, &path);
	is_external = cmd(tab, path, env, &cleanup);
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
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
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
	if ((*env)->env->error_code == 131)
		ft_putstr_fd("Quit/ (Core dumped)\n", STDERR_FILENO);
}

void	execute_builtin_command(t_ast *node, t_ast **head, t_ast **env,
		t_cmd_params *params)
{
	char				**tab;
	char				*path;
	t_cleanup_params	cleanup;

	tab = NULL;
	path = NULL;
	cleanup.head = head;
	cleanup.env = env;
	redirect_io2(params->input_fd, params->output_fd);
	prepare_command_args(node, &cleanup, &tab, &path);
	build_in(tab, 0, env, &cleanup);
	if (tab)
		free_argv_shallow(tab);
	if (path)
		free(path);
}
