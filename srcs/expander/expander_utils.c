/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 18:09:07 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/15 17:01:25 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	remove_empty_elements(char **arr)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		if (arr[i] != NULL && ft_strncmp(arr[i], "", 1) != 0)
			arr[j++] = arr[i];
		i++;
	}
	arr[j] = NULL;
}

int	home(t_global *g, char **value, size_t i, char *tmp)
{
	char	*str;

	tmp = check_malloc(ft_strdup(""));
	str = *value;
	while (str[i])
	{
		if (str[i] == '~' && !g->single_quotes && ! g->double_quotes)
		{
			if ((i > 0 && !is_white_space((str[i - 1])))
				|| (!is_white_space(str[i + 1]) && str[i + 1] != '\0'
					&& str[i + 1] != '/'))
				tmp = check_malloc(ft_strjoin(tmp, "~"));
			else
			{
				if (!env_finder("HOME"))
					return (1);
				tmp = check_malloc(ft_strjoin(tmp, env_finder("HOME")->value));
			}
		}
		else
			tmp = check_malloc(ft_strjoin(tmp, "~"));
		toggle_quote(str[i], &g->single_quotes, &g->double_quotes);
		i++;
	}
	return (*value = tmp, 1);
}

char	*append_literal(char *tmp, char *token_value, size_t *start, size_t *i)
{
	tmp = ft_strappend(tmp, &token_value[*start], *i - *start);
	return (tmp);
}

int	dollar(t_global	*g, char **value)
{
	char	*tmp;
	size_t	i;
	size_t	start;

	i = 0;
	tmp = check_malloc(ft_strdup(""));
	while ((*value)[i])
	{
		if ((*value)[i] == '$' && !g->single_quotes)
			tmp = dollar_sign(tmp, (*value), &i, g);
		else
		{
			start = i;
			while ((*value)[i] && !((*value)[i] == '$' && !g->single_quotes))
				toggle_quote((*value)[i++],
					&g->single_quotes, &g->double_quotes);
			tmp = check_malloc(append_literal(tmp, *value, &start, &i));
		}
	}
	return (*value = tmp, 1);
}
