/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:45:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/26 14:57:31 by agaroux          ###   ########.fr       */
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

char	*create_env_entry(char *name, char *value)
{
	char	*temp;
	char	*new_entry;

	temp = ft_strjoin(name, "=");
	if (!temp)
		return (NULL);
	new_entry = ft_strjoin(temp, value);
	free(temp);
	return (new_entry);
}

void	set_env_var(t_ast *env, char *name, char *value)
{
	int		i;
	int		name_len;
	char	*new_entry;

	if (!env || !env->env || !env->env->env || !name || !value)
		return ;
	name_len = ft_strlen(name);
	new_entry = create_env_entry(name, value);
	if (!new_entry)
		return ;
	i = 0;
	while (env->env->env[i])
	{
		if (ft_strncmp(env->env->env[i], name, name_len) == 0
			&& env->env->env[i][name_len] == '=')
		{
			free(env->env->env[i]);
			env->env->env[i] = new_entry;
			return ;
		}
		i++;
	}
	free(new_entry);
}
