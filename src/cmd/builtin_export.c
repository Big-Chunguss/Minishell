/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:45:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 15:41:30 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

int	parse_exp(char *argv)
{
	int	j;

	j = skip_isspace(argv);
	if (argv[j] == '=')
		return (1);
	if (argv[j] >= '0' && argv[j] <= '9')
		return (1);
	if (check_dbl_equal(argv))
		return (1);
	while (isalpha(argv[j]) || isalnum(argv[j]) || argv[j] == '_')
		j++;
	if (argv[j] == '=' || argv[j] == '\0')
		return (0);
	if (argv[j] != '\0' && argv[j] != '=')
		return (1);
	return (1);
}

static void	export_error(char *var_name)
{
	write(2, "export: `", 9);
	write(2, var_name, ft_strlen(var_name));
	write(2, "': not a valid identifier\n", 26);
}

static void	handle_export_variable(char *argv, t_ast **env)
{
	int	is_assign;

	is_assign = (strchr(argv, '=') != NULL);
	if (!is_assign)
		add_export(argv, env);
	else
	{
		add_env(argv, env);
		add_export(argv, env);
	}
}

void	process_export_args(char **argv, int *i, t_ast **env, int *invalid)
{
	while (argv[*i])
	{
		if (parse_exp(argv[*i]) == 1)
		{
			export_error(argv[*i]);
			*invalid = 1;
		}
		else
			handle_export_variable(argv[*i], env);
		(*i)++;
	}
}

void	export_recognition(char **argv, int i, t_ast **env)
{
	int	invalid;

	invalid = 0;
	i++;
	if (argv[i] == NULL)
	{
		show_export(env);
		return ;
	}
	process_export_args(argv, &i, env, &invalid);
	if (invalid)
		(*env)->env->error_code = 1;
	else
		(*env)->env->error_code = 0;
}
