/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:16:51 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/27 23:45:21 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_type(char *str)
{
	if (!ft_strcmp(str, "|"))
		return (PIPE);
	else if (!ft_strcmp(str, "<"))
		return (INPUT_REDIRECT);
	else if (!ft_strcmp(str, "<<"))
		return (HEREDOC);
	else if (!ft_strcmp(str, ">"))
		return (OUTPUT_REDIRECT);
	else if (!ft_strcmp(str, ">>"))
		return (APPEND);
	else if (contains_meta_character(str))
		return (INVALID);
	else
		return (WORD);
}

t_ast	*create_ast(int type, char *word)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	node->type = type;
	node->value = word;
	node->children = NULL;
	node->child_count = 0;
	return (node);
}

int	contains_meta_character(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (is_meta_character(str[i]))
			return (1);
		i++;
	}
	return (0);
}

int	is_meta_character(char c)
{
	int		i;
	char	*str;

	i = 0;
	str = " \t\n|&;()<>";
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}
