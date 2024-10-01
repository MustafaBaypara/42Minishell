/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:23:38 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/01 14:52:27 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	expand(t_global *g)
{
	size_t	i;
	char	**str;

	i = 0;
	g->single_quotes = 0;
	g->double_quotes = 0;
	str = g->cmd_list->value;
	while (str[i])
	{
		if (ft_strnstr(str[i], "$", ft_strlen(str[i])))
			if (!dollar(g, &str[i]))
				return (1);
		// if (ft_strnstr(str[i], "~", ft_strlen(str[i])))
		// 	if (!home(g, &str[i]))
		// 		return (1);
		printf("%s\n", str[i]);
		i++;
	}
	return (1);
}

void	expander(t_global *g)
{
	t_command	*cmd;

	cmd = g->cmd_list;
	while (cmd)
	{
		if (expand(g))
			return (g->error_no = 12, error_program(0, 12));
		// if (expand_rdr(g, cmd))
		// 	return (g->error_no = 12, error_program(0, 12));
		cmd = cmd->next;
	}
}
