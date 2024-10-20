/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 15:12:50 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/20 14:47:21 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	space_cleaner(t_global *g, t_list *prev)
{
	t_list	*tmp;
	t_list	*head;

	tmp = g->token_list;
	head = g->token_list;
	while (tmp)
	{
		if (!check_space((char *)tmp->content))
		{
			if (head == tmp)
				g->token_list = tmp->next;
			else
				prev->next = tmp->next;
			remove_from_list(&g->garbage_list, tmp);
			if (prev)
				tmp = prev->next;
			else
				tmp = g->token_list;
		}
		else
		{
			prev = tmp;
			tmp = tmp->next;
		}
	}
}

static int	check_quote(t_list *token, int sq, int dq)
{
	size_t	q_double;
	size_t	q_single;
	size_t	i;
	char	*content;

	i = -1;
	q_double = 0;
	q_single = 0;
	content = (char *)token->content;
	while (content[++i])
	{
		if (content[i] == '\'' && !dq)
		{
			q_single++;
			sq = !sq;
		}
		if (content[i] == '"' && !sq)
		{
			q_double++;
			dq = !dq;
		}
	}
	if (q_double % 2 != 0 || q_single % 2 != 0)
		return (0);
	return (1);
}

static int	check_pipe(t_list *token, size_t i)
{
	if (identifier(token->content) != PIPE)
		return (1);
	if (token->next == NULL)
		return (0);
	else if (identifier(token->next->content) == PIPE)
		return (0);
	else if (i == 0 && token->next != NULL)
		return (0);
	return (1);
}

static int	check_rdr(t_list *token, size_t i)
{
	t_token_types	type;

	type = identifier(token->content);
	if (type != RDR_IN && type != RDRD_IN && type != RDR_OUT && type != HEREDOC)
		return (1);
	if (token->next)
		type = identifier(token->next->content);
	if (token->next == NULL)
		return (0);
	else if (identifier(token->next->content) == PIPE)
		return (0);
	else if (type == RDR_IN || type == RDRD_IN
		|| type == RDR_OUT || type == HEREDOC)
		return (0);
	else if (i == 0 && token->next == NULL)
		return (0);
	return (1);
}

int	check_syntax(t_global *g)
{
	t_list	*token;
	size_t	i;
	size_t	j;

	space_cleaner(g, NULL);
	token = g->token_list;
	i = ft_lstsize(token);
	j = i;
	while (token)
	{
		if (!check_quote(token, 0, 0))
			return (ft_putstr_fd("syntax error near unexpected quote.\n", 2), 0);
		if (!check_pipe(token, i - j))
			return (ft_putstr_fd("syntax error near unexpected token.\n", 2),
				0);
		else if (!check_rdr(token, i - j))
			return (ft_putstr_fd("syntax error near unexpected token.\n", 2),
				0);
		j--;
		token = token->next;
	}
	return (1);
}
