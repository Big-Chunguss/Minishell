/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chained_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:24:00 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/29 18:28:12 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/// @brief Creates a new token node with quote information
/// @param str String value for the token (takes ownership)
/// @param was_quoted Flag indicating if the token was originally quoted
/// @return Pointer to newly created token, or NULL on allocation failure
t_token	*ft_lstnew_with_quote_info(char *str, int was_quoted)
{
	t_token	*element;

	element = malloc(sizeof(t_token));
	if (!element)
	{
		if (str)
			free(str);
		return (NULL);
	}
	element->value = str;
	element->was_quoted = was_quoted;
	if (was_quoted)
		element->type = WORD;
	else
		element->type = check_type(str);
	element->next = NULL;
	element->prev = NULL;
	return (element);
}

/// @brief Creates a token linked list from an array of token info structures
/// @param start Pointer to pointer to the first token (will be set by function)
/// @param tokens Array of token info structures containing values and quote flags
/// @param token_count Number of tokens in the array
/// @return 1 on success, 0 on allocation failure
int	create_list_with_quote_info(t_token **start, t_token_info *tokens,
		int token_count)
{
	t_token	*new;
	int		i;
	char	*str_copy;

	*start = NULL;
	i = 0;
	while (i < token_count)
	{
		str_copy = ft_strdup(tokens[i].value);
		if (!str_copy)
		{
			free_stack(start);
			return (0);
		}
		new = ft_lstnew_with_quote_info(str_copy, tokens[i].was_quoted);
		if (!new)
		{
			free(str_copy);
			free_stack(start);
			return (0);
		}
		ft_lstadd_back_with_quote_info(start, new);
		i++;
	}
	return (1);
}

void	ft_lstadd_back(t_token **lst, t_token *new, char *str)
{
	t_token	*current;

	new->value = str;
	new->type = check_type(str);
	new->was_quoted = 0;
	new->next = NULL;
	new->prev = NULL;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
	new->prev = current;
}
