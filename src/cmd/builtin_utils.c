/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:45:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 15:41:30 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_exp_fnc(t_ast *current, char **temp, char *argv)
{
	int	i;

	i = 0;
	while (current->env->export && current->env->export[i])
		i++;
	temp = malloc(sizeof(char *) * (i + 2));
	if (!temp)
		return ;
	i = 0;
	while (current->env->export && current->env->export[i])
	{
		temp[i] = current->env->export[i];
		i++;
	}
	current->env->error_code = 0;
	temp[i] = cat_dup(argv);
	temp[i + 1] = NULL;
	free(current->env->export);
	current->env->export = temp;
	return ;
}

void	add_export(char *argv, t_ast **env)
{
	t_ast	*current;
	char	**temp;

	temp = NULL;
	if (!env || !*env || !(*env)->env || !argv)
		return ;
	current = *env;
	if (!current->env->export)
	{
		temp = malloc(sizeof(char *) * 2);
		if (!temp)
			return ;
		temp[0] = cat_dup(argv);
		temp[1] = NULL;
		current->env->export = temp;
		(*env)->env->error_code = 0;
		return ;
	}
	add_exp_fnc(current, temp, argv);
	return ;
}
