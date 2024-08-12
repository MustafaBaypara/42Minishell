/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbagecollector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 16:50:28 by mbaypara          #+#    #+#             */
/*   Updated: 2024/07/29 17:47:24 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "stdlib.h"

void	*add_list(t_list *list, void *garbage)
{
	t_list		*new_node;

	new_node = ft_lstnew(garbage);
	if (!new_node)
		return (error_program(ERROR_MALLOC, 12), NULL);
	ft_lstadd_back(&list, new_node);
	return (garbage);
}

void	clear_garbage(t_list **lst)
{
	t_list	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->content);
		free(*lst);
		*lst = tmp;
	}
	return ;
}
