/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:45:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 15:29:34 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	build_in(char **tab, int i, t_ast **env)
{
	if (ft_strnstr(BUILTIN, tab[i]))
	{
		if (ft_strnstr("pwd", tab[i]))
			pwd_recognition(env);
		if (ft_strnstr("env", tab[i]))
			env_recognition(tab, i, env);
		if (ft_strnstr("cd", tab[i]))
			cd_recognition(tab, i, env);
		if (ft_strnstr("export", tab[i]))
			export_recognition(tab, i, env);
		if (ft_strnstr("unset", tab[i]))
			unset_recognition(tab, i, env);
		if (ft_strnstr("echo", tab[i]))
			echo_recognition(tab, i, env);
		if (ft_strnstr("exit", tab[i]))
			exit_recognition(tab, i, env);
		return ;
	}
	return ;
}

int	cmd(char **tab, char *path, t_ast **env)
{
	int	i;

	i = 0;
	if (ft_strnstr(BUILTIN, tab[i]) == 1)
	{
		build_in(tab, i, env);
		free(tab);
		free(path);
		return (0);
	}
	return (1);
}

int	cmd_recognize(char *tab)
{
	if (ft_strnstr(BUILTIN, tab) == 1)
		return (0);
	return (1);
}
