/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:38:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 23:13:44 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	unset_exp_fnc(t_ast *current, char *target, char **temp, int j)
{
	int	i;

	i = 0;
	while (current->env->export && current->env->export[i])
	{
		if (ft_strncmp(current->env->export[i], target, ft_strlen(target)))
		{
			temp[j] = current->env->export[i];
			j++;
		}
		else
		{
			free(current->env->export[i]);
		}
		i++;
	}
	temp[j] = NULL;
	free(current->env->export);
	current->env->export = temp;
	return ;
}

void	unset_env_fnc(t_ast *current, char *argv, char **temp, int j)
{
	int	i;

	i = 0;
	while (current->env->env && current->env->env[i])
	{
		if (ft_strncmp(current->env->env[i], argv, ft_strlen(argv)) != 0
			|| (current->env->env[i][ft_strlen(argv)] != '='
			&& current->env->env[i][ft_strlen(argv)] != '\0'))
		{
			temp[j] = current->env->env[i];
			j++;
		}
		else
		{
			free(current->env->env[i]);
		}
		i++;
	}
	temp[j] = NULL;
	free(current->env->env);
	current->env->env = temp;
	return ;
}

int	tab_len(t_ast *current)
{
	int	count;

	count = 0;
	if (!current || !current->env || !current->env->export)
		return (0);
	while (current->env->export && current->env->export[count])
		count++;
	return (count);
}

void	free_both(char *target, t_ast *current)
{
	free(target);
	free(current->env->export);
}

int	search_value(char *str, t_ast **env)
{
	t_ast	*current;
	int		j;
	int		count;
	char	*target;

	j = 0;
	count = 0;
	current = *env;
	target = cat_dup(str);
	while (current->env->export[j])
	{
		if (ft_strncmp(current->env->export[j], (target),
				ft_strlen(target)) != 0)
			j++;
		else
		{
			count++;
			j++;
		}
	}
	free(target);
	if (count > 0)
		return ((*env)->env->error_code = 1, 0);
	(*env)->env->error_code = 0;
	return (1);
}
