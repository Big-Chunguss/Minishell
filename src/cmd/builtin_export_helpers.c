/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:35:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 15:41:30 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_dbl_equal(char *argv)
{
	int	i;
	int	temp;

	i = 0;
	temp = 0;
	while (argv[i])
	{
		if (argv[i] == '=')
			temp++;
		i++;
	}
	if (temp > 1)
		return (1);
	return (0);
}

int	skip_isspace(char *argv)
{
	int	i;

	i = 0;
	while (argv[i] == 32 || argv[i] == '\t' || argv[i] == '\n')
		i++;
	return (i);
}

char	*cat_dup(char *s1)
{
	if (!s1)
		return (NULL);
	return (ft_strjoin("export ", s1));
}

void	create_initial_env(char *argv, t_ast **env)
{
	t_ast	*current;
	char	**temp;

	current = *env;
	temp = malloc(sizeof(char *) * 2);
	if (!temp)
		return ;
	temp[0] = ft_strdup(argv);
	temp[1] = NULL;
	current->env->env = temp;
}

void	add_env(char *argv, t_ast **env)
{
	t_ast	*current;
	int		i;
	char	**temp;

	i = 0;
	if (!env || !*env || !(*env)->env || !argv)
		return ;
	current = *env;
	if (!current->env->env)
	{
		create_initial_env(argv, env);
		return ;
	}
	while (current->env->env && current->env->env[i])
		i++;
	temp = malloc(sizeof(char *) * (i + 2));
	if (!temp)
		return ;
	add_env_fnc(current, temp, argv);
}
