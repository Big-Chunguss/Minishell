/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:45:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 15:41:30 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <limits.h>
#include <stdlib.h>

extern int	g_exit_code;

int	handle_home_directory(t_ast **env)
{
	char	*home;

	home = get_env_var(env, "HOME");
	if (!home)
	{
		write(2, "cd: HOME not set\n", 17);
		return (1);
	}
	if (chdir(home) == 0)
		return (0);
	write(2, "cd: ", 4);
	write(2, home, ft_strlen(home));
	write(2, ": No such file or directory\n", 28);
	return (1);
}

int	handle_specific_path(char *path, char *resolved_path)
{
	(void)resolved_path;
	return (change_to_path(path));
}

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
