/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd6.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 18:10:14 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/25 14:12:01 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <limits.h>
#include <stdlib.h>

extern int	g_exit_code;

static int	count_cd_args(char **argv, int i)
{
	int	arg_count;

	arg_count = 0;
	while (argv[i + 1 + arg_count])
		arg_count++;
	return (arg_count);
}

void	cd_recognition(char **argv, int i, t_ast **env)
{
	char	*resolved_path;
	char	*path;

	if (count_cd_args(argv, i) > 1)
	{
		write(2, "cd: too many arguments\n", 23);
		(*env)->env->error_code = 1;
		return ;
	}
	if (!argv || !argv[i + 1])
		path = NULL;
	else
		path = argv[i + 1];
	resolved_path = malloc(PATH_MAX);
	if (!resolved_path)
	{
		(*env)->env->error_code = 1;
		return ;
	}
	if (path == NULL || ft_strcmp(path, "~") == 0)
		(*env)->env->error_code = handle_home_directory(env);
	else
		(*env)->env->error_code = handle_specific_path(path, resolved_path);
	free(resolved_path);
}

void	print_error(int num, char *tab, t_ast **env)
{
	if (!env || !*env || !(*env)->env)
		return ;
	if (num == 1)
	{
		if (!tab)
			write(2, "cd: No such file or directory\n", 30);
		else
		{
			write(2, "cd: ", 4);
			write(2, tab, ft_strlen(tab));
			write(2, ": No such file or directory\n", 28);
		}
		(*env)->env->error_code = 1;
		return ;
	}
	if (num == 2)
	{
		write(2, "cd: ", 4);
		write(2, tab, ft_strlen(tab));
		write(2, ": Permission denied\n", 20);
		(*env)->env->error_code = 1;
		return ;
	}
	return ;
}

int	access_error(char *tab)
{
	if (!tab)
		return (1);
	if (access(tab, F_OK) != 0)
		return (1);
	if (access(tab, R_OK) != 0)
		return (2);
	return (0);
}

void	pwd_change(char *pwd, char *oldpwd, t_ast **env)
{
	char	*pw;
	char	*oldpw;

	if (!env || !*env || !(*env)->env || !pwd || !oldpwd)
		return ;
	unset_env("OLDPWD=", env);
	unset_exp("OLDPWD=", env);
	unset_env("PWD=", env);
	unset_exp("PWD=", env);
	pw = ft_strjoin("PWD=", pwd);
	oldpw = ft_strjoin("OLDPWD=", oldpwd);
	if (pw)
	{
		add_env(pw, env);
		add_export(pw, env);
		free(pw);
	}
	if (oldpw)
	{
		add_env(oldpw, env);
		add_export(oldpw, env);
		free(oldpw);
	}
	return ;
}
