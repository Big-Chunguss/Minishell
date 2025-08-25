/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:45:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 15:29:34 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	pwd_recognition(t_ast **env)
{
	int		i;
	int		j;
	t_ast	*current;

	i = 0;
	j = 4;
	current = *env;
	if (!current->env->env[i])
		return ;
	while (current->env->env[i] && ft_strncmp(current->env->env[i], "PWD=", 4))
		i++;
	if (current->env->env[i] && !ft_strncmp(current->env->env[i], "PWD=", 4))
	{
		while (current->env->env[i][j] != '\0')
		{
			write(1, &current->env->env[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
	return ;
}
