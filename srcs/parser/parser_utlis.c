/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utlis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:35:12 by mbaypara          #+#    #+#             */
/*   Updated: 2024/09/13 15:43:12 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	skip_quote(char *val, size_t *i, char quote)
{
	(*i)++;
	while (val[*i] && val[*i] != quote)
		(*i)++;
	if (val[*i] == quote)
		(*i)++;
}

static size_t	token_value(char *val)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (val[i])
	{
		while (val[i] && is_white_space(val[i]))
			i++;
		while (val[i] && !is_white_space(val[i]))
		{
			if (val[i] == '\'')
				skip_quote(val, &i, '\'');
			else if (val[i] == '"')
				skip_quote(val, &i, '"');
			else
				i++;
		}
		len++;
	}
	return (len);
}

size_t	token_len(t_list *list)
{
	size_t	len;

	len = 0;
	while (list && identifier(list->content) != PIPE)
	{
		len += token_value(list->content);
		list = list->content;
	}
	return (len);
}
