/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:23:38 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/02 18:08:20 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	remove_quotes_inplace(char **c_val)
{
	size_t	i;
	size_t	j;
	int		in_s;
	int		in_d;

	while (*c_val)
	{
		i = 0;
		j = 0;
		in_s = 0;
		in_d = 0;
		while ((*c_val)[i])
		{
			toggle_quote((*c_val)[i], &in_s, &in_d);
			if (((*c_val)[i] == '\'' && !in_d) || ((*c_val)[i] == '"' && !in_s))
				i++;
			else
				(*c_val)[j++] = (*c_val)[i++];
		}
		(*c_val)[j] = '\0';
		c_val++;
	}
}

static int	expand(t_global *g, t_command *clst)
{
	size_t	i;
	char	**str;

	i = 0;
	g->single_quotes = 0;
	g->double_quotes = 0;
	str = clst->value;
	while (str[i])
	{
		if (ft_strnstr(str[i], "$", ft_strlen(str[i])))
			if (!dollar(g, &str[i]))
				return (1);
		if (ft_strnstr(str[i], "~", ft_strlen(str[i])))
			if (!home(g, &str[i], 0, NULL))
				return (1);
		i++;
	}
	remove_quotes_inplace(clst->value);
	return (0);
}

static int	redirects(char **str, t_global *g, size_t *i)
{
	char	*tmp;

	if (ft_strnstr(str[*i + 1], "$", ft_strlen(str[*i + 1])))
		if (!dollar(g, &str[*i + 1]))
			return (1);
	if (ft_strnstr(str[*i + 1], "~", ft_strlen(str[*i + 1])))
		if (!home(g, &str[*i + 1], 0, NULL))
			return (1);
	if (!str[*i + 1][0] || (ft_strnstr(str[*i + 1], " ", ft_strlen(str[*i + 1]))
		&& !((ft_strnstr(str[*i + 1], "'", ft_strlen(str[*i + 1])))
			|| (ft_strnstr(str[*i + 1], "\"", ft_strlen(str[*i + 1]))))))
		return (-1);
	tmp = quote_clean(str[*i + 1], 0, 0);
	str[*i + 1] = tmp;
	*i += 2;
	return (0);
}

static int	rdr(t_global *g, t_command *clst)
{
	size_t		i;
	int			res;

	i = 0;
	g->single_quotes = 0;
	g->double_quotes = 0;
	while (clst->rds && clst->rds[i] && clst->rds[i + 1])
	{
		res = redirects(clst->rds, g, &i);
		if (res == 1)
			return (1);
		if (res == -1)
		{
			clst->is_work = 0;
			ft_putendl_fd("minishell: ambiguous redirect", 2);
			return (0);
		}
	}
	return (0);
}

void	expander(t_global *g)
{
	t_command	*cmd;

	if (g->control == 0)
		return ;
	cmd = g->cmd_list;
	while (cmd)
	{
		if (expand(g, cmd))
			return (g->error_no = 12, error_program(0, 12));
		if (rdr(g, cmd))
			return (g->error_no = 12, error_program(0, 12));
		cmd = cmd->next;
	}
}
