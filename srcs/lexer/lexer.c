/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 16:47:05 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/04 14:01:48 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	listing(t_global *g, size_t *j, size_t *i)
{
	char	*current;

	current = &g->command_line[*i];
	if (*j > 0)
	{
		g->tmp[*j] = '\0';
		add_list(&g->token_list, check_malloc(ft_strdup(g->tmp)));
		*j = 0;
		add_list(&g->garbage_list, ft_lstlast(g->token_list));
	}
	if (*current == '>' && current[1] == '>')
	{
		(*i)++;
		add_list(&g->token_list, check_malloc(ft_strdup(">>")));
	}
	else if (*current == '<' && current[1] == '<')
	{
		(*i)++;
		add_list(&g->token_list, check_malloc(ft_strdup("<<")));
	}
	else
		add_list(&g->token_list, check_malloc(ft_substr(current, 0, 1)));
	add_list(&g->garbage_list, ft_lstlast(g->token_list));
}

static void	lexer_definer(t_global *g, size_t i_len, size_t *j)
{
	size_t	i;
	char	*cmd;

	i = 0;
	cmd = g->command_line;
	while (i < i_len)
	{
		if ((cmd[i] == '|' || cmd[i] == '<' || cmd[i] == '>')
			&& !g->single_quotes && !g->double_quotes)
			listing(g, j, &i);
		else if (cmd[i] == '"' && !g->single_quotes)
		{
			g->double_quotes = !g->double_quotes;
			g->tmp[(*j)++] = cmd[i];
		}
		else if (cmd[i] == '\'' && !g->double_quotes)
		{
			g->single_quotes = !g->single_quotes;
			g->tmp[(*j)++] = cmd[i];
		}
		else
			g->tmp[(*j)++] = cmd[i];
		i++;
	}
}

void	lexer(t_global *g)
{
	size_t	i_len;
	size_t	j;

	j = 0;
	i_len = ft_strlen(g->command_line);
	g->tmp = check_malloc(ft_calloc(1, (i_len + 1) * sizeof(char)));
	lexer_definer(g, i_len, &j);
	if (j > 0)
	{
		g->tmp[j] = '\0';
		add_list(&g->token_list, check_malloc(ft_strdup(g->tmp)));
		add_list(&g->garbage_list, ft_lstlast(g->token_list));
	}
}
