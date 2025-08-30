/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:10:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/29 18:17:22 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/// @brief Initializes pipeline execution data structure
/// @param data Pipeline data structure to initialize
/// @param node AST node representing the start of the pipeline
/// @param input_fd File descriptor for pipeline input
static void	init_pipeline_vars(t_pipeline_data *data, t_ast *node, int input_fd)
{
	data->cmds = count_pipeline_cmds(node);
	data->idx = 0;
	data->cur_in = input_fd;
	data->cur = node;
}

/// @brief Executes a complete command pipeline with proper process management
/// @param node AST node representing the pipeline to execute
/// @param head Pointer to AST head for context
/// @param env Pointer to environment AST
/// @param params Command execution parameters including file descriptors
static void	run_pipeline(t_ast *node, t_ast **head, t_ast **env,
		t_cmd_params *params)
{
	t_pipeline_data	data;
	t_cmd_params	cmd_params;
	pid_t			pid;
	pid_t			last_pid;
	int				cmd_count;

	init_pipeline_vars(&data, node, params->input_fd);
	cmd_count = data.cmds;
	run_pipeline_loop(head, env, &data);
	pid = fork();
	if (pid == 0)
	{
		cmd_params.input_fd = data.cur_in;
		cmd_params.output_fd = params->output_fd;
		run_command(data.cur, head, env, &cmd_params);
	}
	last_pid = pid;
	if (data.cur_in != STDIN_FILENO)
		close(data.cur_in);
	wait_for_all_pipeline_children(cmd_count, last_pid, env);
}

void	exec2_exec_ast(t_ast **node, t_ast **head, t_ast **env,
		t_cmd_params *params)
{
	if (!node || !*node)
		return ;
	if ((*node)->type == NODE_PIPE)
		run_pipeline(*node, head, env, params);
	else if ((*node)->type == NODE_COMMAND)
		handle_command_execution(*node, head, env, params);
}

void	execute_nodes(t_ast **head, t_ast **env)
{
	int				saved_stdout;
	int				saved_stdin;
	t_cmd_params	params;

	if (!head || !*head)
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
