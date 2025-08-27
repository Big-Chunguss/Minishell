/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:45:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 15:47:45 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

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

static void	export_error(char *var_name)
{
	write(2, "export: `", 9);
	write(2, var_name, ft_strlen(var_name));
	write(2, "': not a valid identifier\n", 26);
}

int	value_exists_in_export(t_ast **env, char *search)
{
	int		i;
	int		name_len;
	char	*va_start;

	if (!env || !(*env) || !(*env)->env || !(*env)->env->export || !search)
		return (0);
	name_len = ft_strlen(search);
	printf("%s\n", search);
	i = 0;
	while ((*env)->env->export[i])
	{
		printf("%s", (*env)->env->export[i]);
		va_start = (*env)->env->export[i] + 7;
		if (ft_strncmp(va_start, search, name_len) == 0
			&& (va_start[name_len] == '=' || va_start[name_len] == '\0'))
			return (1);
		i++;
	}
	return (0);
}

static void	handle_export_variable(char *argv, t_ast **env)
{
	int		is_assign;
	char	*var_name;
	char	*equals_pos;
	int		name_len;
	int		j;

	is_assign = (strchr(argv, '=') != NULL);
	if (!is_assign && value_exists_in_export(env, argv))
		return ;
	if (!is_assign)
		add_export(argv, env);
	else
	{
		equals_pos = strchr(argv, '=');
		name_len = equals_pos - argv;
		var_name = malloc(name_len + 1);
		j = -1;
		while (++j < name_len)
			var_name[j] = argv[j];
		var_name[name_len] = '\0';
		unset_env(var_name, env);
		unset_exp(var_name, env);
		add_env(argv, env);
		add_export(argv, env);
		free(var_name);
	}
}

void	process_export_args(char **argv, int *i, t_ast **env, int *invalid)
{
	while (argv[*i])
	{
		if (parse_exp(argv[*i]) == 1)
		{
			export_error(argv[*i]);
			*invalid = 1;
		}
		else
			handle_export_variable(argv[*i], env);
		(*i)++;
	}
}

void	export_recognition(char **argv, int i, t_ast **env)
{
	int	invalid;

	invalid = 0;
	i++;
	if (argv[i] == NULL)
	{
		show_export((env));
		return ;
	}
	process_export_args(argv, &i, env, &invalid);
	if (invalid)
		(*env)->env->error_code = 1;
	else
		(*env)->env->error_code = 0;
}
