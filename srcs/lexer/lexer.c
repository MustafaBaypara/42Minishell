/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 16:47:05 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/20 16:56:37 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	check_space(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] == ' ')
		i++;
	if (i != 0 && str[i] == '\0')
		return (0);
	return (1);
}

static void	listing(t_global *g, size_t *j, size_t *i)
{
	char	*current;

	current = &g->command_line[*i];

	// kelime varsa token listesine ekler
	if (*j > 0)
	{
		g->tmp[*j] = '\0';
		add_list(&g->token_list, check_malloc(ft_strdup(g->tmp)));
		*j = 0;
		add_list(&g->garbage_list, ft_lstlast(g->token_list));
	}
	// çift yönlenme karakterlerini ayırır
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
	else // tek yönlenme karakterlerini ayırır
		add_list(&g->token_list, check_malloc(ft_substr(current, 0, 1)));
	// listeye ekler
	add_list(&g->garbage_list, ft_lstlast(g->token_list));
}

static void	lexer_definer(t_global *g, size_t i_len, size_t *j)
{
	size_t	i;
	char	*cmd;

	i = 0;
	cmd = g->command_line;

	// tokenları ayırırken single ve double quotes içindeki karakterleri ayırır
	while (i < i_len)
	{
		if ((cmd[i] == '|' || cmd[i] == '<' || cmd[i] == '>') // ayırımda karakter kontrolü
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
	// kalan son kelimesi ayırır ve listeye ekler
	if (j > 0)
	{
		g->tmp[j] = '\0';
		add_list(&g->token_list, check_malloc(ft_strdup(g->tmp)));
		add_list(&g->garbage_list, ft_lstlast(g->token_list));
	}
	// syntax kontrolü yapar
	if (!check_syntax(g)) // olmazsa döngüye döner ve değişkenleri sıfırlar
	{
		g->control = 0; // kontorlü kapatır ve ilerideki işlemleri yapmaz
		g->error_no = 2;
		clean_list(&g->token_list); // listeyi temizler
		remove_from_list(&g->garbage_list, g->command_line); // garbage listesinden komut satırını çıkarır
		g->command_line = NULL;
		g->token_list = NULL;
	}
}
