/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:45:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 19:37:56 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	number_not_valid(char **argv, int i, t_ast **env, t_cleanup_params *cleanup)
{
	(*env)->env->error_code = 2;
	write(2, "minishell: exit: ", 17);
	write(2, argv[i + 1], ft_strlen(argv[i + 1]));
	write(2, ": numeric argument required\n", 28);
	cleanup_readline_resources();
	cleanup_and_exit(2, cleanup);
}

static void	handle_exit_with_arg(char **argv, int i, t_ast **env, t_cleanup_params *cleanup)
{
	if (!is_valid_number(argv[i + 1]))
		number_not_valid(argv, i, env, cleanup);
	else if (check_has_sign(argv[i + 1]))
		number_has_sign(argv, i, env, cleanup);
	else
	{
		(*env)->env->error_code = ft_atoi(argv[i + 1]) % 256;
		cleanup_readline_resources();
		cleanup_and_exit((*env)->env->error_code, cleanup);
	}
}

static void	handle_exit_no_arg(t_ast **env)
{
	if (g_exit_code >= 128)
		(*env)->env->error_code = g_exit_code;
	cleanup_readline_resources();
	ft_putstr_fd("minishell: exit\n", STDERR_FILENO);
	exit(g_exit_code);
}

void	exit_recognition(char **argv, int i, t_ast **env, t_cleanup_params *cleanup)
{
	int	a;

	a = 0;
	while (argv[a])
		a++;
	if (a > 2)
	{
		too_much_exit(env);
		return ;
	}
	if (argv[i + 1])
		handle_exit_with_arg(argv, i, env, cleanup);
	else
		handle_exit_no_arg(env);
}
