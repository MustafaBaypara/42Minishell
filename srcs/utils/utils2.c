/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 20:35:53 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/19 18:48:07 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_split_first(const char *s, char c)
{
	char	**result;
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i] && s[i] != c)
		i++;
	result = malloc(3 * sizeof(char *));
	if (!result)
		return (NULL);
	result[0] = ft_substr(s, 0, i);
	if (!result[0])
		return (free(result), NULL);
	result[1] = ft_substr(s + i + (s[i] == c), 0,
			ft_strlen(s) - i - (s[i] == c));
	if (!result[1])
	{
		free(result[0]);
		free(result);
		return (NULL);
	}
	result[2] = NULL;
	return (result);
}

t_token_types	identifier(char *content)
{
	if (ft_strlen(content) == 1)
	{
		if (*content == '|')
			return (PIPE);
		else if (*content == '<')
			return (RDR_OUT);
		else if (*content == '>')
			return (RDR_IN);
	}
	else if (ft_strlen(content) == 2)
	{
		if (*content == '<' && content[1] == '<')
			return (HEREDOC);
		if (*content == '>' && content[1] == '>')
			return (RDRD_IN);
	}
	return (CMD);
}

int	is_white_space(char c)
{
	if (c == 32 || (9 <= c && c <= 13))
		return (1);
	return (0);
}

char	*quote_clean(char *str, int s, int d)
{
	char	*res;
	size_t	i;
	size_t	j;

	res = check_malloc(ft_calloc(ft_strlen(str) + 1, sizeof(char)));
	j = 0;
	i = 0;
	while (str[i])
	{
		toggle_quote(str[i], &s, &d);
		if ((str[i] == '\'' && !d) || (str[i] == '"' && !s))
		{
			i++;
			continue ;
		}
		res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}

int	check_hdoc(t_command *cmd)
{
	int	i;
	int	j;

	catch_signal(5);
	j = 0;
	while (cmd)
	{
		i = 0;
		while (cmd->rds && cmd->rds[i])
			if (!ft_strncmp(cmd->rds[i++], "<<", 2))
				j++;
		cmd = cmd->next;
	}
	if (j == 0)
		return (0);
	return (1);
}
