/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 16:47:05 by mbaypara          #+#    #+#             */
/*   Updated: 2024/07/29 18:18:35 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	listing(t_global *g, size_t *j, size_t *i)
{
	char	*current;

	current = NULL;
	*current = g->command_line[*i];

	if (*j > 0)
	{
		g->tmp[*j] = '\0';
		add_list(g->token_list, g->tmp);
		*j = 0;
	}
	if (*current == '>' && g->command_line[*i + 1] == '>')
		add_list(g->token_list, ">>");
	if (*current == '<' && g->command_line[*i + 1] == '<')
		add_list(g->token_list, "<<");
	add_list(g->token_list, current);
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
	g->tmp = check_malloc((char *)malloc((i_len + 1) * sizeof(char)));
	lexer_definer(g, i_len, &j);
	printf("%s\n", g->command_line);
	printf("%s\n", (char *)g->token_list->content);
}
