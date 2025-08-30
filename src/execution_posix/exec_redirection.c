/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:33:49 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/30 17:03:41 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

/// @brief Handles input redirection for a command
/// @param child AST node representing the input redirection
/// @return 0 on success, -1 on error
int	handle_input_redirection(t_ast *child)
{
	int	fd;

	fd = open_input_redir(child);
	if (fd == -1)
		return (-1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

/// @brief Handles output redirection for a command
/// @param child AST node representing the output redirection
/// @return 0 on success, -1 on error
int	handle_output_redirection(t_ast *child)
{
	int	fd;

	fd = open_output_redir(child);
	if (fd == -1)
		return (-1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	process_redirection_child(t_ast *child)
{
	if (g_exit_code == 130)
		return (-1);
	if (!ft_strcmp(child->value, "<"))
		return (handle_input_redirection(child));
	else if (!ft_strcmp(child->value, "<<"))
		return (handle_input_redirection(child));
	else if (!ft_strcmp(child->value, ">") || !ft_strcmp(child->value, ">>"))
		return (handle_output_redirection(child));
	return (0);
}
