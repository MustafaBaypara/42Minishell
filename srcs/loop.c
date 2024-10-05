/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:31:54 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/05 18:35:15 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>

static int	line_reader(t_global *g)
{
	char	*tmp;

	tmp = readline("minishell> ");
	if (!tmp)
		return (error_program(0, 1), (0));
	g->command_line = check_malloc(ft_strtrim(tmp, " "));
	free(tmp);
	add_history(g->command_line);
	if (!check_space(g->command_line))
		return (0);
	if (ft_strncmp(g->command_line, "exit", 5) == 0)
		return (-1);
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

		int x = 0;
		t_list *tmp = g->garbage_list;
		while (tmp)
		{
			x++;
			tmp = tmp->next;
		}
		//printf("%d\n", x);
		
	}
	rl_clear_history();
	return ;
}
