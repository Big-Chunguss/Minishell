/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:42:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/26 14:44:19 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	add_env_fnc(t_ast *current, char **temp, char *argv)
{
	int	i;

	i = 0;
	while (current->env->env && current->env->env[i])
	{
		temp[i] = current->env->env[i];
		i++;
	}
	temp[i] = ft_strdup(argv);
	temp[i + 1] = NULL;
	free(current->env->env);
	current->env->env = temp;
	return ;
}

void	initialise_exp(t_ast **env, char **envp)
{
	t_ast	*current;
	int		count;

	if (!envp)
		return ;
	current = *env;
	if (!current || !current->env)
		return ;
	count = count_envp_entries(envp);
	if (!allocate_export_array(current, count))
		return ;
	copy_envp_entries(env, envp);
}
