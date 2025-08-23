/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:59:42 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/23 12:37:31 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	unset_env(char *argv, t_ast **env)
{
	t_ast	*current;
	char	**temp;
	int		j;
	int		count;

	j = 0;
	count = 0;
	if (!env || !*env || !(*env)->env || !argv || !(*env)->env->env)
		return ;
	current = *env;
	while (current->env->env[count])
		count++;
	temp = malloc(sizeof(char *) * (count + 1));
	if (!temp)
		return ;
	unset_env_fnc(current, argv, temp, j);
	return ;
}

char	*cat_dup(char *s1)
{
	if (!s1)
		return (NULL);
	return (ft_strjoin("export ", s1));
}

void	export_recognition(char **argv, int i, t_ast **env)
{
	int	invalid;
	int	is_assign;

	i++;
	invalid = 0;
	if (argv[i] == NULL)
	{
		show_export(env);
		return ;
	}
	while (argv[i])
	{
		is_assign = (strchr(argv[i], '=') != NULL);
		if (parse_exp(argv[i]) == 1)
		{
			write(2, "export: `", 9);
			write(2, argv[i], ft_strlen(argv[i]));
			write(2, "': not a valid identifier\n", 26);
			invalid = 1;
		}
		else if (!is_assign)
			add_export(argv[i], env);
		else
		{
			add_env(argv[i], env);
			add_export(argv[i], env);
		}
		i++;
	}
	(*env)->env->error_code = invalid ? 1 : 0;
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
		temp = malloc(sizeof(char *) * 2);
		if (!temp)
			return ;
		temp[0] = ft_strdup(argv);
		temp[1] = NULL;
		current->env->env = temp;
		return ;
	}
	while (current->env->env && current->env->env[i])
		i++;
	temp = malloc(sizeof(char *) * (i + 2));
	if (!temp)
		return ;
	add_env_fnc(current, temp, argv);
	return ;
}

int	parse_exp(char *argv)
{
	int	j;

	j = skip_isspace(argv);
	if (argv[j] == '=')
		return (1);
	if (argv[j] >= '0' && argv[j] <= '9')
		return (1);
	if (check_dbl_equal(argv))
		return (1);
	while (isalpha(argv[j]) || isalnum(argv[j]) || argv[j] == '_')
		j++;
	if (argv[j] == '=' || argv[j] == '\0')
		return (0);
	if (argv[j] != '\0' && argv[j] != '=')
		return (1);
	return (1);
}
