/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:31:54 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/07 14:12:15 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>

static int	line_reader(t_global *g)
{
	g->command_line = readline("minishell> ");
	if (!g->command_line)
		return (rl_clear_history(), error_program(0, 1), (-1));
	else if (!g->command_line[0])
		return (free(g->command_line), 0);
	else if (!check_space(g->command_line))
		return (free(g->command_line), 0);
	else
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
		executor(g);
		catch_signal(1);
		g->control = 1;
	}
}
