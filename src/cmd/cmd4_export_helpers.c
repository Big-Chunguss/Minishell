/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd4_export_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:00:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 13:09:06 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	create_initial_env(char *argv, t_ast **env)
{
	t_ast	*current;
	char	**temp;

	current = *env;
	temp = malloc(sizeof(char *) * 2);
	if (!temp)
		return ;
	temp[0] = ft_strdup(argv);
	temp[1] = NULL;
	current->env->env = temp;
}
