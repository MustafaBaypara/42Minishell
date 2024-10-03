/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:48:08 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/03 15:23:54 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>

static t_command	*cmd_init(t_list *token)
{
	t_command	*cmd;
	size_t		i;

	if (!token)
		return (NULL);
	cmd = check_malloc(ft_calloc(1, sizeof(t_command)));
	if (identifier(token->content) == PIPE)
		token = token->next;
	i = token_len(token);
	cmd->value = (char **)check_malloc(ft_calloc((i + 1), sizeof(char *)));
	cmd->value[i] = NULL;
	cmd->p_type = 0;
	cmd->fd[0] = STDIN_FILENO;
	cmd->fd[1] = STDOUT_FILENO;
	cmd->pid = -1;
	cmd->the_fd = -1;
	cmd->rds = NULL;
	cmd->next = NULL;
	cmd->is_work = 1;
	return (cmd);
}

static int	get_token(t_command **cmd, t_list **t, size_t *i)
{
	if (!rdr_position(*cmd))
		return (0);
	*i = 0;
	(*cmd)->next = cmd_init(*t);
	if (!(*cmd)->next)
		return (0);
	(*cmd)->p_type = 2;
	*cmd = (*cmd)->next;
	(*cmd)->p_type = 1;
	return (1);
}

static int	get_command(t_command **cmd, t_list **t, size_t *i)
{
	int		d;
	int		s;
	size_t	j;
	size_t	start;
	char	*cleanstr;

	j = 0;
	d = 0;
	s = 0;
	cleanstr = (char *)(*t)->content;
	while (cleanstr[j])
	{
		while (cleanstr[j] && is_white_space(cleanstr[j]) && !d && !s)
			j++;
		start = j;
		if (cleanstr[j])
		{
			j = word_end(cleanstr, j, &d, &s);
			cleanstr = check_malloc(ft_substr(cleanstr, start, j - start));
			(*cmd)->value[(*i)++] = cleanstr;
			cleanstr = (char *)(*t)->content;
		}
	}
	return (1);
}

void	parser(t_global *g)
{
	size_t		i;
	t_command	*cmd;
	t_list		*token;

	if (g->control == 0)
		return ;
	i = 0;
	token = g->token_list;
	cmd = cmd_init(token);
	g->cmd_list = cmd;
	while (token)
	{
		if (identifier(token->content) != PIPE)
			get_command(&cmd, &token, &i);
		else
			get_token(&cmd, &token, &i);
		token = token->next;
	}
	rdr_position(cmd);
	if (!heredocs(g, g->cmd_list))
		return (g->control = 0, error_program(0, g->error_no));
	return (cmd->next = NULL, (void)0);
}
