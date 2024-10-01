/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 18:09:07 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/01 14:52:11 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	home(t_global *g, char **value)
{
	char	*str;
	char	*tmp;
	size_t	i;

	(void)g;
	(void)i;
	tmp = check_malloc(ft_strdup(""));
	str = *value;
	return 4;
}

int	dollar(t_global	*g, char **value)
{
	char	*tmp;
	char	*str;
	size_t	i;
	size_t	start;

	i = 0;
	tmp = check_malloc(ft_strdup(""));
	str = *value;
	while (str[i])
	{
		if (str[i] == '$' && !g->single_quotes)
			tmp = dollar_sign(tmp, str, &i, g);
		else
		{
			start = i;
			while (str[i] && !(str[i] == '$' && !g->single_quotes))
				toggle_quote(str[i++], &g->single_quotes, &g->double_quotes);
			tmp = check_malloc(ft_strjoin(tmp, \
			check_malloc(ft_substr(str, start, i))));
		}
	}
	return (*value = tmp, 1);
}
