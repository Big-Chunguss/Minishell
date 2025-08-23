/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd1_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 15:00:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 13:10:08 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_envp_entries(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

int	allocate_export_array(t_ast *current, int count)
{
	current->env->export = malloc(sizeof(char *) * (count + 1));
	if (!current->env->export)
		return (0);
	return (1);
}

int	copy_envp_entries(t_ast **env, char **envp)
{
	t_ast	*current;
	int		i;

	current = *env;
	i = 0;
	while (envp[i])
	{
		current->env->export[i] = cat_dup(envp[i]);
		if (!current->env->export[i])
		{
			del_export(env);
			return (0);
		}
		i++;
	}
	current->env->export[i] = NULL;
	return (1);
}
