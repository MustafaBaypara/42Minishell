/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:27:24 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/21 16:50:38 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_dsign(char *tmp)
{
	t_global	*g;
	char		*num;

	g = _global(NULL);
	num = check_malloc(ft_itoa(g->error_no));
	tmp = check_malloc(ft_strjoin(tmp, num));
	return (tmp);
}

static char	*expand_var(char *tmp, char *t_val, size_t *i)
{
	size_t	j;
	char	*new_tmp;
	t_env	*e_v;

	j = 0;
	(*i)++;
	if (ft_isdigit(t_val[*i]) || t_val[*i +1] == '"' || t_val[*i +1] == '\'')
		return ((*i)++, tmp);
	while ((ft_isalnum(t_val[*i + j]) || t_val[*i + j] == '_'))
		j++;
	new_tmp = check_malloc(ft_strndup(&t_val[*i], j));
	e_v = env_finder(new_tmp);
	if (e_v)
		tmp = check_malloc(ft_strjoin(tmp, e_v->value));
	*i += j;
	return (tmp);
}

char	*dollar_sign(char *tmp, char *t_val, size_t *i, t_global *g)
{
	if (t_val[*i + 1] == '?')
	{
		tmp = expand_dsign(tmp);
		*i += 2;
	}
	else if (t_val[*i + 1] == '\0')
	{
		tmp = check_malloc(ft_strjoin(tmp, "$"));
		(*i)++;
	}
	else if (ft_isalnum(t_val[*i + 1]) || t_val[*i + 1] == '_' \
	|| ((t_val[*i + 1] == '"' || t_val[*i + 1] == '\'') \
	&& !g->single_quotes && !g->double_quotes))
		tmp = expand_var(tmp, t_val, i);
	else
	{
		tmp = check_malloc(ft_strjoin(tmp, "$"));
		(*i)++;
	}
	return (tmp);
}
