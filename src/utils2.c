/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:20:23 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/29 17:50:59 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_var_char(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'
		|| (c >= '0' && c <= '9'));
}

char	*unquoted_var_expansion(char *str, t_ast **env)
{
	int	i;
	int	in_single_quotes;
	int	in_double_quotes;
	int	replaced_first;

	in_single_quotes = 0;
	in_double_quotes = 0;
	replaced_first = 0;
	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		handle_quotes(str, i, &in_single_quotes, &in_double_quotes);
		if (!in_single_quotes && handle_var_expansion(&str, &i, env,
				&replaced_first))
			continue ;
		i++;
	}
	return (str);
}
