/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_helpers3.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:43:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 22:32:16 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_pwd_at_startup(t_ast *env)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return ;
	set_env_var(env, "PWD", cwd);
	free(cwd);
}

void	update_pwd_after_cd(t_ast *env, char *old_pwd)
{
	char	*new_pwd;

	if (old_pwd)
		set_env_var(env, "OLDPWD", old_pwd);
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		set_env_var(env, "PWD", new_pwd);
		free(new_pwd);
	}
}

char	*get_env_var(t_ast **env, char *str)
{
	int	i;
	int	len;

	if (!env || !(*env) || !(*env)->env || !(*env)->env->env)
		return (NULL);
	len = ft_strlen(str);
	i = 0;
	while ((*env)->env->env[i])
	{
		if (ft_strncmp((*env)->env->env[i], str, len) == 0
			&& (*env)->env->env[i][len] == '=')
			return ((*env)->env->env[i] + len + 1);
		i++;
	}
	return (NULL);
}

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
