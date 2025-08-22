/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:12:41 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/22 14:37:11 by agaroux          ###   ########.fr       */
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
	
	// Pre-validate all commands before any execution/forking
	if (!pre_validate_ast(*head, env))
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
	char	*path;
	int		result;

	if (!cmd)
		return (0);
	
	// Check if it's a built-in command first (cmd_recognize returns 0 for built-ins)
	if (cmd_recognize((char *)cmd) == 0)
		return (1);
	
	// Get the command path for external commands
	path = get_cmd_path(cmd, env);
	if (!path)
		return (0);
	
	// Check if file exists and is executable
	result = (access(path, F_OK) == 0 && !is_directory(path) && access(path, X_OK) == 0);
	free(path);
	return (result);
}

int	pre_validate_ast(t_ast *node, t_ast **env)
{
	if (!node)
		return (1);
	
	if (node->type == NODE_COMMAND && node->value)
	{
		if (!validate_command_exists(node->value, env))
		{
			command_not_found_error(node->value);
			(*env)->env->error_code = 127;
			return (0);
		}
	}
	
	// Recursively validate children
	if (node->left && !pre_validate_ast(node->left, env))
		return (0);
	if (node->right && !pre_validate_ast(node->right, env))
		return (0);
	
	return (1);
}
