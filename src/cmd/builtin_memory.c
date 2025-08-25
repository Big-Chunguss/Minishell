/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_memory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:45:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 15:29:34 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_env_complete(t_ast *env)
{
	int	i;

	i = 0;
	if (!env || !env->env)
		return ;
	if (env->env->env)
		free_env_env(env);
	if (env->env->export)
	{
		while (env->env->export[i])
		{
			if (env->env->export[i])
				free(env->env->export[i]);
			i++;
		}
		free(env->env->export);
	}
	free(env->env);
	free(env);
}

void	free_env_env(t_ast *env)
{
	int	i;

	i = 0;
	while (env->env->env[i])
	{
		if (env->env->env[i])
			free(env->env->env[i]);
		i++;
	}
	free(env->env->env);
}

void	del_export(t_ast **env)
{
	t_ast	*current;
	int		i;

	i = 0;
	current = *env;
	if (!current || !current->env || !current->env->export)
		return ;
	while (current->env->export[i])
	{
		free(current->env->export[i]);
		i++;
	}
	free(current->env->export);
	current->env->export = NULL;
	return ;
}
