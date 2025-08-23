/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:10:09 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/10 11:42:24 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/minishell.h"

char	*expand_variable(char *str, t_ast **env)

{
	int		start;
	int		len;
	char	*res;
	if (find_next_expand(str, &start, &len))
	{
		res = expand_one(str, start, len, env);
		free(str);
		if (ft_strchr(res, '$'))
			return (expand_variable(res, env));
		return (res);
	}
	return (str);
}
static int	is_valid_var_char(char c)

{
	if ((c >= 'A' && c <= 'Z') || (c == '_'))
		return (1);
	return (0);
}
static int	get_var_name_length(const char *str, int i)

{
	int	j;
	j = 1;
	while (str[i + j] && is_valid_var_char(str[i + j]))
		j++;
	return (j);
}
static int	is_valid_expand_context(int sq, int dq)

{
	return (dq % 2 == 1 && sq % 2 == 0);
}
int	find_next_expand(const char *str, int *start, int *len)

{
	int	i;
	int	sq;
	int	dq;
	i = 0;
	sq = 0;
	dq = 0;
	while (str[i])
	{
		if (str[i] == '\'' && dq % 2 == 0)
			sq++;
		else if (str[i] == '\"' && sq % 2 == 0)
			dq++;
		if (str[i] == '$' && is_valid_expand_context(sq, dq))
		{
			*len = get_var_name_length(str, i);
			*start = i;
			return (1);
		}
		i++;
	}
	return (0);
}
