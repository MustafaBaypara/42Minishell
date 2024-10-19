/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:31:54 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/19 18:19:22 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

static int	line_reader(t_global *g)
{
	g->command_line = readline("minishell> ");
	if (!g->command_line)
		return (rl_clear_history(), error_program("exit", g->error_no), (-1));
	else if (!g->command_line[0])
		return (free(g->command_line), 0);
	else if (!check_space(g->command_line))
		return (free(g->command_line), 0);
	g->command_line = check_malloc(g->command_line);
	add_history(g->command_line);
	return (1);
}

void	loop(t_global *g)
{
	int	i;

	while (1)
	{
		i = line_reader(g);
		if (!i)
			continue ;
		if (i == -1)
			break ;
		lexer(g);
		parser(g);
		expander(g);
		executor(g, 0, 0);
		catch_signal(1);
		g->control = 1;
	}
}
