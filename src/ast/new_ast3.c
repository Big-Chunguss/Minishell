/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_ast3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:12:57 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/21 19:45:23 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../includes/minishell.h"

t_ast	**combine(t_ast **head, t_ast *cmd)

{
	t_ast	**new_head;
	if (!head)
	{
		head = malloc(sizeof(t_ast *));
		*head = NULL;
	}
	if (!*head)
	{
		*head = cmd;
		return (head);
	}
	else
	{
		new_head = malloc(sizeof(t_ast *));
		*new_head = create_ast_node(NODE_PIPE, "|");
		set_ast_left(*new_head, *head);
		set_ast_right(*new_head, cmd);
		return (new_head);
	}
}
void	free_split(char **split)

{
	int	i;
	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
t_ast	**build_and_print_ast(t_token *lst, t_ast **env)

{
	t_ast	**root;
	root = malloc(sizeof(t_ast *));
	*root = parse_pipeline(&lst, env);
	return (root);
}
char	*ft_strjoin_slash(char const *s1, char const *s2)

{
	char	*res;
	char	*tmp;
	tmp = ft_strjoin(s1, "/");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, s2);
	if (!res)
	{
		free(tmp);
		return (NULL);
	}
	free(tmp);
	return (res);
}
