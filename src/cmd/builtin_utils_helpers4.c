/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_helpers4.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:43:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/26 15:15:19 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	show_export(t_ast **env)
{
	int		i;
	t_ast	*current;
	int		j;

	i = 0;
	j = 0;
	current = *env;
	if (!current->env->export)
		return ;
	while (current->env->export[i])
	{
		write(1, &current->env->export[i][j], 1);
		j++;
		if (current->env->export[i][j] == '\0')
		{
			write(1, "\n", 1);
			j = 0;
			i++;
		}
	}
	return ;
}

void	show_env(t_ast **env)
{
	int		i;
	t_ast	*current;
	int		j;

	i = 0;
	j = 0;
	current = *env;
	while (current->env->env[i])
	{
		write(1, &current->env->env[i][j], 1);
		j++;
		if (current->env->env[i][j] == '\0')
		{
			write(1, "\n", 1);
			j = 0;
			i++;
		}
	}
	return ;
}

char	*cat_dup(char *s1)
{
	if (!s1)
		return (NULL);
	return (ft_strjoin("export ", s1));
}
