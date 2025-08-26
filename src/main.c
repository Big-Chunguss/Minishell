/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:47:21 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/26 15:21:44 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_exit_code = 0;

void	infinite_read(t_token **lst, t_ast **env)
{
	char	*line;

	while (1)
	{
		line = get_input();
		if (!line)
			return ;
		if (line[0] == 0 || g_exit_code > 128)
		{
			free(line);
			continue ;
		}
		add_history(line);
		if (!ft_strcmp(line, "clear"))
			clear_history();
		if (!ft_strcmp(line, "exit"))
		{
			free(line);
			cleanup_readline_resources();
			break ;
		}
		process_tokens(lst, line, env);
		free(line);
	}
}

char	*get_input(void)
{
	char	*line;
	char	*tmp;

	line = readline("Minishell> ");
	if (line == NULL)
		return (NULL);
	while (open_quotes(line))
	{
		tmp = readline_open_quotes(line);
		if (!tmp)
		{
			free(line);
			return (NULL);
		}
		free(line);
		line = tmp;
	}
	return (line);
}

void	unlink_redirection(t_token **lst)
{
	t_token	*tmp;

	tmp = *lst;
	while (tmp)
	{
		if (!ft_strcmp(tmp->value, "<<"))
			unlink(tmp->next->value);
		tmp = tmp->next;
	}
}

int	main(int argc, char **argv, char **env)
{
	int		final_exit_code;
	t_token	*list;
	t_token	**lst;
	t_ast	**ast_head;
	t_ast	*ast;

	(void)argc;
	(void)argv;
	ast = NULL;
	ast_head = &ast;
	list = NULL;
	lst = &list;
	setup_sigint_handler();
	setup_sigquit_handler();
	disable_echoctl();
	initialise_env(ast_head, env);
	initialise_exp(ast_head, env);
	initialise_shlvl(ast_head);
	infinite_read(lst, ast_head);
	cleanup_readline_resources();
	final_exit_code = ast->env->error_code;
	free_env_complete(ast);
	if (g_exit_code >= 128)
		return (g_exit_code);
	return (final_exit_code % 256);
}

void	cleanup_readline_resources(void)
{
	cleanup_get_next_line();
	rl_clear_history();
	rl_cleanup_after_signal();
	rl_deprep_terminal();
}
