/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:12:41 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 15:05:00 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	command_not_found_error(const char *cmd)
{
	fprintf(stderr, "%s: command not found\n", cmd);
}

void	handle_errno_error(const char *path)
{
	if (errno == EACCES)
	{
		fprintf(stderr, "%s: Permission denied\n", path);
		exit(1);
	}
	else if (errno == ENOENT)
	{
		fprintf(stderr, "%s: No such file or directory\n", path);
		exit(1);
	}
	else
	{
		perror(path);
		exit(1);
	}
}

int	is_directory(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
		return (S_ISDIR(st.st_mode));
	return (0);
}

void	exit_child(int exit_code, int child)
{
	if (child == CHILD)
		exit(exit_code);
}

/// @brief main function that will take the ast
/// node and take care of the execution
/// @param head ASTNode containing the tree
/// @param env
void	execute_nodes(t_ast **head, t_ast **env)
{
	int				saved_stdout;
	int				saved_stdin;
	t_cmd_params	params;

	if (!head || !(*head))
		return ;
	
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	params.input_fd = STDIN_FILENO;
	params.output_fd = STDOUT_FILENO;
	exec2_exec_ast(head, head, env, &params);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
}

int	validate_command_exists(const char *cmd, t_ast **env)
{
	(void)cmd;
	(void)env;
	return (1);
}

int	pre_validate_ast(t_ast *node, t_ast **env)
{
	(void)node;
	(void)env;
	return (1);
}
