/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd6_cd_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/25 14:06:44 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <limits.h>
#include <stdlib.h>

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

int	change_to_path(char *path)
{
	if (chdir(path) == 0)
		return (0);
	write(2, "cd: ", 4);
	write(2, path, ft_strlen(path));
	write(2, ": No such file or directory\n", 28);
	return (1);
}

int	handle_specific_path(char *path, char *resolved_path)
{
	(void)resolved_path;
	return (change_to_path(path));
}
