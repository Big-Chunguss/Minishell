/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:45:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 15:41:30 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	env_recognition(char **tab, int j, t_ast **env)
{
	int		i;
	t_ast	*current;

	current = *env;
	i = 0;
	if (!current->env->env)
		return ;
	if (tab[j + 1])
		return ;
	while (current->env->env[i])
	{
		write(1, current->env->env[i], ft_strlen(current->env->env[i]));
		write(1, "\n", 1);
		i++;
	}
	return ;
}
