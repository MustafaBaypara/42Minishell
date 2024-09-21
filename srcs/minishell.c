/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:46:06 by mbaypara          #+#    #+#             */
/*   Updated: 2024/09/21 16:35:15 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>

t_global	*_global(t_global *g)
{
	static t_global	*global;

	if (g)
		global = g;
	return (global);
}

void	init_global(t_global *g)
{
	_global(g);
	g->garbage_list = NULL;
	g->error_no = 1;
	g->single_quotes = 0;
	g->double_quotes = 0;
	g->token_list = NULL;
	g->cmd_list = NULL;
}

int	main(int ac, char **av, char **env)
{
	t_global	global;

	init_global(&global);
	if (ac != 1 || av[1])
		error_program(ERROR_ARG, 1);
	global.env = env_dup(env, &global);
	if (!global.env)
		return (0);
	loop(&global);
	clear_garbage(&global.garbage_list);
}
