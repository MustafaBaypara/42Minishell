/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:35:12 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/19 18:48:07 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		list = list->next;
	}
	return (len);
}

void	toggle_quote(char c, int *in_s, int *in_d)
{
	if (c == '\'' && !(*in_d))
		*in_s = !(*in_s);
	else if (c == '"' && !(*in_s))
		*in_d = !(*in_d);
}

size_t	word_end(const char *v, size_t start, int *d, int *s)
{
	size_t	j;

	j = start;
	while (v[j])
	{
		toggle_quote(v[j], s, d);
		if (is_white_space(v[j]) && !(*d) && !(*s))
			break ;
		j++;
	}
	return (j);
}
