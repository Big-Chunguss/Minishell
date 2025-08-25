/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_helpers2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:43:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 15:41:30 by agaroux          ###   ########.fr       */
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

char	*number_shlvl(t_ast **env)
{
	t_ast	*current;
	int		i;
	char	*str;
	char	*result;

	i = 0;
	current = *env;
	if (!current->env->env)
		return (NULL);
	while (current->env->env[i])
	{
		if (!(ft_strncmp(current->env->env[i], "SHLVL=", 6)))
		{
			str = current->env->env[i] + 6;
			while (*str && !ft_isdigit(*str))
				str++;
			result = ft_strdup(str);
			return (result);
		}
		i++;
	}
	return (ft_strdup("0"));
}

char	*get_env_var(t_ast **env, char *str)
{
	int	i;
	int	len;

	if (!env || !(*env) || !(*env)->env || !(*env)->env->env)
		return (NULL);
	len = strlen(str);
	i = 0;
	while ((*env)->env->env[i])
	{
		if (strncmp((*env)->env->env[i], str, len) == 0)
			return ((*env)->env->env[i] + len);
		i++;
	}
	return (NULL);
}

void	initialise_shlvl(t_ast **env)
{
	char	*str;
	char	*merge;
	int		shlvl;
	char	*final;

	if (!env || !*env)
		return ;
	str = number_shlvl(env);
	if (!str)
		return ;
	shlvl = ft_atoi(str);
	free(str);
	shlvl++;
	final = ft_itoa(shlvl);
	if (!final)
		return ;
	unset_env("SHLVL=", env);
	unset_exp("SHLVL=", env);
	merge = ft_strjoin("SHLVL=", final);
	free(final);
	if (!merge)
		return ;
	add_env(merge, env);
	free(merge);
}
