/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:35:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 22:32:30 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static char	*get_validated_argv(char *argv)
{
	if (ft_strncmp(argv, "SHLVL=", 6) == 0)
		return (create_validated_shlvl_entry(argv));
	return (argv);
}

static void	handle_initial_env(char *validated_argv, char *argv, t_ast **env)
{
	create_initial_env(validated_argv, env);
	if (validated_argv != argv)
		free(validated_argv);
}

static void	handle_existing_env(char *validated_argv, char *argv, t_ast **env)
{
	t_ast	*current;
	int		i;
	char	**temp;

	current = *env;
	i = 0;
	while (current->env->env && current->env->env[i])
		i++;
	temp = malloc(sizeof(char *) * (i + 2));
	if (!temp)
	{
		if (validated_argv != argv)
			free(validated_argv);
		return ;
	}
	add_env_fnc(current, temp, validated_argv);
	if (validated_argv != argv)
		free(validated_argv);
}

void	add_env(char *argv, t_ast **env)
{
	t_ast	*current;
	char	*validated_argv;

	if (!env || !*env || !(*env)->env || !argv)
		return ;
	current = *env;
	validated_argv = get_validated_argv(argv);
	if (!current->env->env)
	{
		handle_initial_env(validated_argv, argv, env);
		return ;
	}
	handle_existing_env(validated_argv, argv, env);
}
