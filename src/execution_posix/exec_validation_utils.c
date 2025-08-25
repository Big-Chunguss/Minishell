/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_validation_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:25:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 12:56:00 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_directory(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
		return (S_ISDIR(st.st_mode));
	return (0);
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
