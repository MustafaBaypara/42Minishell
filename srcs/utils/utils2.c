/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 20:35:53 by mbaypara          #+#    #+#             */
/*   Updated: 2024/09/13 15:21:48 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
