/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:10:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 12:37:31 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

extern int	g_exit_code;

static void	write_error_msg(const char *cmd, const char *msg)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

static void	print_exec_error(const char *cmd, int code, int has_slash)
{
	if (code == 127)
	{
		if (has_slash)
			write_error_msg(cmd, "No such file or directory");
		else
			write_error_msg(cmd, "command not found");
	}
	else if (code == 126)
	{
		if (is_directory(cmd))
			write_error_msg(cmd, "Is a directory");
		else if (access(cmd, F_OK) == 0 && access(cmd, X_OK) != 0)
			write_error_msg(cmd, "Permission denied");
	}
}

static int	classify_error(const char *cmd, char *resolved)
{
	int	has_slash;

	has_slash = (strchr(cmd, '/') != NULL);
	if (!resolved)
		return (127);
	if (is_directory(resolved))
		return (126);
	if (access(resolved, F_OK) == 0 && access(resolved, X_OK) != 0)
		return (126);
	return (0);
}

static void	handle_external_command(char **tab, char *path, t_ast **env,
		t_ast **head)
{
	int			code;
	t_cleanup_params	cleanup;
	int			has_slash;

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

static void	setup_command_environment(t_cmd_params *params)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	redirect_io2(params->input_fd, params->output_fd);
}

static int	prepare_command_args(t_ast *node, t_cleanup_params *cleanup,
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

void	run_command(t_ast *node, t_ast **head, t_ast **env,
		t_cmd_params *params)
{
	char				**tab;
	char				*path;
	int					is_external;
	t_cleanup_params	cleanup;

	// Handle NULL node case (shouldn't happen now) or empty command (redirection-only)
	if (!node || !node->value || ft_strlen(node->value) == 0)
	{
		t_cleanup_params	redir_cleanup;
		
		redir_cleanup.head = head;
		redir_cleanup.env = env;
		redir_cleanup.tab = NULL;
		redir_cleanup.path = NULL;
		setup_command_environment(params);
		// Apply redirections if node exists
		if (node && apply_redirections2(node) == -1)
			cleanup_and_exit(1, &redir_cleanup);
		// For redirection-only commands, just exit successfully
		cleanup_and_exit(0, &redir_cleanup);
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

void	handle_command_execution(t_ast *node, t_ast **head, t_ast **env,
		t_cmd_params *params)
{
	pid_t	pid;
	int		status;

	// Handle redirection-only commands (empty command string)
	if (!node || !node->value || ft_strlen(node->value) == 0)
	{
		pid = fork();
		if (pid == 0)
			run_command(node, head, env, params);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			(*env)->env->error_code = WEXITSTATUS(status);
		return;
	}

	// Check if it's a builtin command - execute in parent process without fork
	if (cmd_recognize(node->value) == 0)
	{
		// Execute builtin in parent process
		execute_builtin_command(node, head, env, params);
		return;
	}

	// For external commands, fork as usual
	pid = fork();
	if (pid == 0)
		run_command(node, head, env, params);
	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		(*env)->env->error_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_exit_code = 128 + WTERMSIG(status);
		(*env)->env->error_code = g_exit_code;
	}
}

void	execute_builtin_command(t_ast *node, t_ast **head, t_ast **env,
		t_cmd_params *params)
{
	char	**tab;
	char	*path;
	t_cleanup_params	cleanup;

	(void)head;  // May not be used for builtins
	tab = NULL;
	path = NULL;
	cleanup.head = head;
	cleanup.env = env;
	
	// Set up I/O redirections if any
	setup_command_environment(params);
	
	// Build command arguments
	prepare_command_args(node, &cleanup, &tab, &path);
	
	// Execute the builtin command (will handle it without exit())
	build_in(tab, 0, env);
	
	// Clean up arguments
	if (tab)
		free_argv_shallow(tab);
	if (path)
		free(path);
}
