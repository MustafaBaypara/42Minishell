/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:04:19 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/04 13:56:11 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_rdr(char *strs)
{
	int	i;
	int	in_s;
	int	in_d;

	i = 0;
	in_s = 0;
	in_d = 0;
	while (strs[i])
	{
		toggle_quote(strs[i], &in_s, &in_d);
		if (!in_s && !in_d)
		{
			if (strs[i] == '<')
				return (1);
			if (strs[i] == '>')
				return (1);
		}
		i++;
	}
	return (0);
}

static void	fill_rds(t_command *cmd, char **tmp, size_t *f)
{
	size_t	i;
	size_t	j;
	char	**val;

	i = 0;
	j = 0;
	val = cmd->value;
	while (val[i])
	{
		if (is_rdr(val[i]))
		{
			cmd->rds[j++] = val[i++];
			cmd->rds[j++] = val[i];
		}
		else
			tmp[(*f)++] = val[i];
		i++;
	}
	tmp[*f] = NULL;
}

static size_t	rdr_count(char **str)
{
	size_t	len;
	size_t	i;

	i = 0;
	len = 0;
	while (str[i])
	{
		if (!ft_strncmp(str[i], "<", 1) && ft_strlen(str[i]) == 1 && ++i)
			len++;
		else if (!ft_strncmp(str[i], ">", 1) && ft_strlen(str[i]) == 1 && ++i)
			len++;
		else if (!ft_strncmp(str[i], ">>", 2) && ft_strlen(str[i]) == 2 && ++i)
			len++;
		else if (!ft_strncmp(str[i], "<<", 2) && ft_strlen(str[i]) == 2 && ++i)
			len++;
		i++;
	}
	len *= 2;
	return (len);
}

int	rdr_position(t_command *cmds)
{
	char	**tmp;
	char	**val;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	val = cmds->value;
	if (rdr_count(val) <= 0)
		return (1);
	while (val[i])
		i++;
	tmp = check_malloc(ft_calloc(i - rdr_count(val) + 1, sizeof(char *)));
	cmds->rds = check_malloc(ft_calloc(rdr_count(val) + 1, sizeof(char *)));
	fill_rds(cmds, tmp, &j);
	cmds->value = tmp;
	return (1);
}
