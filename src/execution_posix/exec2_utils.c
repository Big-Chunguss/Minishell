/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:10:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/21 19:45:23 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

extern int	g_exit_code;

void	free_ast_full(t_ast **head)
{
	if (head && *head)
	{
		ast_free(*head);
		*head = NULL;
	}
}

void	free_argv_shallow(char **tab)
{
	if (tab)
		free(tab);
}

void	cleanup_and_exit(int status, t_cleanup_params *params)
{
	if (params->tab)
		free_argv_shallow(params->tab);
	if (params->path)
		free(params->path);
	if (params->env && *(params->env))
		free_env_complete(*(params->env));
	free_ast_full(params->head);
	exit(status);
}

void	redirect_io2(int input_fd, int output_fd)
{
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
}

int	count_pipeline_cmds(t_ast *node)
{
	int		count;
	t_ast	*cur;

	count = 0;
	cur = node;
	while (cur && cur->type == NODE_PIPE)
	{
		count++;
		cur = cur->right;
	}
	if (cur)
		count++;
	return (count);
}
