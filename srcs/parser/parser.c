/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:48:08 by mbaypara          #+#    #+#             */
/*   Updated: 2024/09/13 17:44:09 by mbaypara         ###   ########.fr       */
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
	cmd->redirects = NULL;
	cmd->is_work = 1;
	return (cmd);
}

void	parser(t_global *g, t_list *token, size_t i, t_command *cmd)
{
	cmd = cmd_init(token);
	add_list(&g->cmd_list, cmd);
	while (token)
	{
		if (identifier(token->content) != PIPE)
		{
			if (!get_command())
		}
	}
}
