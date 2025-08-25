/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:01:46 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/25 13:09:06 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

static void	handle_exit_with_arg(char **argv, int i, t_ast **env)
{
	if (!is_valid_number(argv[i + 1]))
		number_not_valid(argv, i, env);
	else if (check_has_sign(argv[i + 1]))
		number_has_sign(argv, i, env);
	else
	{
		(*env)->env->error_code = ft_atoi(argv[i + 1]) % 256;
		cleanup_readline_resources();
		exit((*env)->env->error_code);
	}
}

static void	handle_exit_no_arg(t_ast **env)
{
	if (g_exit_code >= 128)
		(*env)->env->error_code = g_exit_code;
	cleanup_readline_resources();
	exit((*env)->env->error_code);
}

void	exit_recognition(char **argv, int i, t_ast **env)
{
	int	a;

	a = 0;
	while (argv[a])
		a++;
	if (a > 2)
		too_much_exit(env);
	if (argv[i + 1])
		handle_exit_with_arg(argv, i, env);
	else
		handle_exit_no_arg(env);
}

void	echo_recognition(char **argv, int i, t_ast **env)
{
	int	count;

	count = 1;
	i++;
	(*env)->env->error_code = 0;
	if (argv[i] && ft_strcmp(argv[i], "-n") == 0)
	{
		count = 0;
		i++;
	}
	while (argv[i])
	{
		write(1, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (count)
		write(1, "\n", 1);
	return ;
}
