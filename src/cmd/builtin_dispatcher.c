/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:45:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 19:30:54 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	build_in(char **tab, int i, t_ast **env, t_cleanup_params *cleanup)
{
	if (ft_strnstr(BUILTIN, tab[i]))
	{
		if (ft_strcmp("pwd", tab[i]) == 0)
			pwd_recognition(env);
		else if (ft_strcmp("env", tab[i]) == 0)
			env_recognition(tab, i, env);
		else if (ft_strcmp("cd", tab[i]) == 0)
			cd_recognition(tab, i, env);
		else if (ft_strcmp("export", tab[i]) == 0)
			export_recognition(tab, i, env);
		else if (ft_strcmp("unset", tab[i]) == 0)
			unset_recognition(tab, i, env);
		else if (ft_strcmp("echo", tab[i]) == 0)
			echo_recognition(tab, i, env);
		else if (ft_strcmp("exit", tab[i]) == 0)
			exit_recognition(tab, i,env, cleanup);
	}
}

int	cmd(char **tab, char *path, t_ast **env, t_cleanup_params *cleanup)
{
	int	i;

	i = 0;
	if (ft_strnstr(BUILTIN, tab[i]) == 1)
	{
		build_in(tab, i, env, cleanup);
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
