/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:30:19 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 13:10:07 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


static char	*safe_substr(const char *str, int start, int len)

{
	char	*result;
	result = ft_substr(str, start, len);
	if (!result)
		result = ft_strdup("");
	return (result);
}
static void	extract_var_parts(char **parts, const char *str, int start, int len)

{
	parts[0] = safe_substr(str, 0, start);
	parts[1] = safe_substr(str, start + 1, len - 1);
	parts[2] = ft_strdup(str + start + len);
	if (!parts[2])
		parts[2] = ft_strdup("");
}
static char	*join_expanded_parts(char **parts, char *val)

{
	char	*tmp;
	char	*res;
	tmp = ft_strjoin(parts[0], val);
	if (!tmp)
		tmp = ft_strdup("");
	res = ft_strjoin(tmp, parts[2]);
	if (!res)
	{
		free(tmp);
		res = ft_strdup("");
	}
	else
	{
		free(tmp);
	}
	return (res);
}
char	*expand_one(const char *str, int start, int len, t_ast **env)

{
	char	*parts[3];
	char	*val;
	char	*res;
	if (!str || start < 0 || len <= 1)
	{
		if (str)
			return (ft_strdup(str));
		else
			return (ft_strdup(""));
	}
	extract_var_parts(parts, str, start, len);
	val = get_value(parts[1], len - 1, env);
	if (!val)
		val = ft_strdup("");
	res = join_expanded_parts(parts, val);
	free(parts[0]);
	free(parts[1]);
	free(parts[2]);
	free(val);
	return (res);
}
