/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbagecollector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 16:50:28 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/04 13:33:14 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "stdlib.h"

void	*add_list(t_list **list, void *garbage)
{
	t_list		*new_node;

	new_node = ft_lstnew(garbage);
	if (!new_node)
		return (error_program(ERROR_MALLOC, 12), NULL);
	ft_lstadd_back(list, new_node);
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
		if ((*lst)->content)
			free((*lst)->content);
		free(*lst);
		*lst = tmp;
	}
	return ;
}

void	remove_from_list(t_list **list, void *dirty)
{
	t_list	*prev;
	t_list	*current;

	prev = NULL;
	current = *list;
	if (current->content == dirty)
	{
		*list = current->next;
		current->next = NULL;
		clear_garbage(&current);
		return ;
	}
	while (current && current->content != dirty)
	{
		prev = current;
		current = current->next;
	}
	if (current && current->content == dirty)
	{
		prev->next = current->next;
		current->next = NULL;
		clear_garbage(&current);
	}
}

void	clean_list(t_list **list)
{
	t_global	*g;
	t_list		*current;

	g = _global(NULL);
	current = NULL;
	current = *list;
	while (current)
	{
		remove_from_list(&g->garbage_list, current->content);
		current = current->next;
	}
}
