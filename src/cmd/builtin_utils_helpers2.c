/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_helpers2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:43:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/26 15:17:13 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	validate_shlvl_export(int value)
{
	if (value < 0)
		return (0);
	else if (value > 1000)
		return (1);
	return (value);
}

char	*create_validated_shlvl_entry(char *argv)
{
	char	*value_str;
	int		value;
	char	*validated_str;
	char	*result;

	value_str = ft_strchr(argv, '=');
	if (!value_str)
		return (ft_strdup(argv));
	value_str++;
	value = ft_atoi(value_str);
	value = validate_shlvl_export(value);
	validated_str = ft_itoa(value);
	if (!validated_str)
		return (ft_strdup(argv));
	result = ft_strjoin("SHLVL=", validated_str);
	free(validated_str);
	if (result)
		return (result);
	return (ft_strdup(argv));
}

static int	verify_shlvl_value(int i)
{
	if (i < 0)
		i = 0;
	else if (i > 1000)
		i = 1;
	else
		i++;
	return (i);
}

void	initialise_shlvl(t_ast **env)
{
	char	*current_shlvl;
	int		shlvl_value;
	char	*new_shlvl;
	char	*shlvl_entry;

	current_shlvl = get_env_var(env, "SHLVL");
	if (!current_shlvl)
		shlvl_value = 1;
	else
	{
		shlvl_value = ft_atoi(current_shlvl);
		shlvl_value = verify_shlvl_value(shlvl_value);
	}
	unset_exp("SHLVL", env);
	new_shlvl = ft_itoa(shlvl_value);
	shlvl_entry = ft_strjoin("SHLVL=", new_shlvl);
	add_env(shlvl_entry, env);
	free(new_shlvl);
	free(shlvl_entry);
}
