/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:45:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 14:56:23 by agaroux          ###   ########.fr       */
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

static void	unset_from_array(char **array, char *name, int name_len)
{
    int	i;
    int	j;
	char *va_start;

    if (!array)
        return ;
    i = 0;
    while (array[i])
    {
		va_start = array[i] + 7;
        if (ft_strncmp(va_start, name, name_len) == 0
            && (va_start[name_len] == '=' || va_start[name_len] == '\0'))
        {
            free(array[i]);
            j = i;
            while (array[j + 1])
            {
                array[j] = array[j + 1];
                j++;
            }
            array[j] = NULL;
            i--;
        }
        i++;
    }
}

void	unset_exp(char *name, t_ast **env)
{
	int	name_len;

	if (!env || !*env || !(*env)->env || !name)
		return ;
	name_len = 0;
	while (name[name_len] && name[name_len] != '=')
		name_len++;
	unset_from_array((*env)->env->export, name, name_len);
}

void	unset_recognition(char **argv, int i, t_ast **env)
{
	i++;
	if (argv[i] == NULL)
	{
		(*env)->env->error_code = 0;
		return ;
	}
	while (argv[i])
	{
		unset_env(argv[i], env);
		unset_exp(argv[i], env);
		i++;
	}
	(*env)->env->error_code = 0;
}
