/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:23:38 by mbaypara          #+#    #+#             */
/*   Updated: 2024/09/29 18:48:51 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	expand(t_global *g, t_command *cmd)
{
	size_t	i;
	char	**str;

	i = 0;
	g->single_quotes = 0;
	g->double_quotes = 0;
	str = cmd->value;
	printf("here\n");
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

void	expander(t_global *g, t_command *cmd)
{

	while (cmd)
	{
		if (expand(g, cmd))
			return (g->error_no = 12, error_program(0, 12));
		// if (expand_rdr(g, cmd))
		// 	return (g->error_no = 12, error_program(0, 12));
		cmd = cmd->next;
	}
}
