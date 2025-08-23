/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_newline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 15:50:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/23 12:02:10 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"

static void	free_result(char **result, int j)

{
	while (--j >= 0)
		free(result[j]);
	free(result);
}
static char	**fill_result(char *str, char **result)

{
	int	i;
	int	j;
	int	start;
	int	len;
	i = 0;
	j = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == '\n')
		{
			len = i - start;
			result[j] = malloc(len + 1);
			if (!result[j])
				return (free_result(result, j), NULL);
			ft_strlcpy(result[j], str + start, len + 1);
			j++;
			start = i + 1;
		}
		i++;
	}
	return (finish_last_line(str, result, j, start));
}
char	**split_newline_alloc(char *str, int line_count)

{
	char	**result;
	result = malloc(sizeof(char *) * (line_count + 1));
	if (!result)
		return (NULL);
	return (fill_result(str, result));
}
char	**finish_last_line(char *str, char **result, int j, int start)

{
	int	i;
	int	len;
	i = 0;
	while (str[i])
		i++;
	if (start < i)
	{
		len = i - start;
		result[j] = malloc(len + 1);
		if (!result[j])
			return (free_result(result, j), NULL);
		ft_strlcpy(result[j], str + start, len + 1);
		j++;
	}
	result[j] = NULL;
	return (result);
}
