/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:30:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 23:12:25 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_has_sign(char *str)
{
	if (!str)
		return (0);
	return (str[0] == '-' || str[0] == '+');
}

void	exit_number_fail(char *str)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	g_exit_code = 255;
	exit(255);
}

void	too_much_exit(t_ast **env)
{
	(*env)->env->error_code = 1;
	ft_putstr_fd("exit\nminishell: exit : too many arguments\n", STDERR_FILENO);
}

void	number_has_sign(char **argv, int i, t_ast **env,
		t_cleanup_params *cleanup)
{
	(*env)->env->error_code = (256 + ft_atoi(argv[i + 1])) % 256;
	cleanup_readline_resources();
	cleanup_and_exit((*env)->env->error_code, cleanup);
}
