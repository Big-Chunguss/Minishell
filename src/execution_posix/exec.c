/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:12:41 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 13:08:43 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int	g_exit_code;

void	command_not_found_error(const char *cmd)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", 20);
}

void	handle_errno_error(const char *path)
{
	char	*msg;

	write(2, path, ft_strlen(path));
	write(2, ": ", 2);
	if (errno == EACCES)
	{
		write(2, "Permission denied\n", 18);
		exit(1);
	}
	else if (errno == ENOENT)
	{
		write(2, "No such file or directory\n", 26);
		exit(1);
	}
	else
	{
		msg = strerror(errno);
		write(2, msg, ft_strlen(msg));
		write(2, "\n", 1);
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
