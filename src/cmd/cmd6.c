/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd6.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 18:10:14 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/23 12:37:31 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"

extern int	g_exit_code;
void	cd_recognition(char **tab, int i, t_ast **env)

{
	int	error_code;
	if (tab[i])
		i++;
	if (!tab[i])
	{
		cd_only(env);
		return ;
	}
	if (tab[i + 1])
	{
		write(2, "cd: too many arguments\n", 23);
		(*env)->env->error_code = 1;
		return ;
	}
	error_code = access_error(tab[i]);
	if (error_code == 0)
		cd_rec_fnc(tab[i], env);
	else
		print_error(error_code, tab[i], env);
	return ;
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
