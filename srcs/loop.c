/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:31:54 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/03 18:10:18 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>

size_t	check_space(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	if (i != 0 && str[i] == '\0')
		return (0);
	return (1);
}

static int	line_reader(t_global *g)
{
	char	*tmp;

	tmp = readline("minishell> ");
	if (!tmp)
		return (error_program(0, 1), (0));
	g->command_line = check_malloc(ft_strtrim(tmp, " "));
	free(tmp);
	if (!g->command_line[0])
		return (0);
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
		printf("%d\n", x);
		
	}
	clear_history();
	return ;
}
