/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chained_list2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:24:00 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/23 13:02:10 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_lstadd_back_with_quote_info(t_token **lst, t_token *new,
		int was_quoted)

{
	t_token	*current;
	(void)was_quoted;
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
int	ft_lstsize(t_token *lst)

{
	int	i;
	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
void	free_stack(t_token **stack)

{
	t_token	*current;
	t_token	*next;
	if (!stack || !*stack)
		return ;
	current = *stack;
	while (current != NULL)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	*stack = NULL;
}
