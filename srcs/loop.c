/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:31:54 by mbaypara          #+#    #+#             */
/*   Updated: 2024/08/25 17:27:26 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

static int	line_reader(t_global *g)
{
	char	*tmp;

	tmp = readline("minishell> ");
	if (!tmp)
		return (error_program("ERROR: Failed readline\n", 1), (0));
	g->command_line = check_malloc(ft_strtrim(tmp, " "));
	free(tmp);
	add_history(g->command_line);
	if (ft_strncmp(g->command_line, "bitir", 5) == 0)
		return (0);
	return (1);
}

void	loop(t_global *g)
{
	int	i;

	while (1)
	{
		i = line_reader(g);
		if (!i)
			break ;
		lexer(g);
	}
	rl_clear_history();
	return ;
}
