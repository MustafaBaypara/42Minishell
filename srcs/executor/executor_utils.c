/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:35:13 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/16 14:21:57 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <sys/stat.h>
#include <unistd.h>

static int	check_path(t_command *cmd, t_global *g, int *i)
{
	struct stat	buf;

	if (!check_slash(cmd->value[0]))
		return (0);
	if (stat(cmd->value[0], &buf) == 0)
	{
		if (S_ISDIR(buf.st_mode))
			return (catch_error(cmd, 4, g), *i = 1, 0);
		if (access(cmd->value[0], X_OK) != 0)
			return (catch_error(cmd, 3, g), *i = 1, 0);
		return (1);
	}
	else
	{
		catch_error(cmd, 2, g);
		*i = 1;
	}
	return (0);
}

static int	check_dir(char *path, t_global *g, t_command *cmd)
{
	struct stat	buf;

	if (stat(path, &buf) == 0)
		if (S_ISDIR(buf.st_mode))
			return (catch_error(cmd, 1, g), 1);
	return (0);
}

static int	path_command(t_command *cmd, t_global *g)
{
	char	*tmp;
	size_t	i;

	i = 0;
	tmp = check_malloc(ft_strjoin("/", cmd->value[0]));
	while (g->path[i])
	{
		cmd->cmdpath = check_malloc(ft_strjoin(g->path[i], tmp));
		if (check_dir(cmd->cmdpath, g, cmd))
			return (cmd->cmdpath = NULL, 0);
		if (access(cmd->cmdpath, X_OK) == 0)
			return (1);
		remove_from_list(&g->garbage_list, cmd->cmdpath);
		cmd->cmdpath = NULL;
		i++;
	}
	return (catch_error(cmd, 1, g), 0);
}

int	is_command_ok(t_command *cmd, t_global *g)
{
	int	i;

	i = 0;
	if (cmd && (!cmd->value[0] || !cmd->value[0][0]))
		return (0);
	if (check_path(cmd, g, &i))
	{
		cmd->cmdpath = check_malloc(ft_strdup(cmd->value[0]));
		return (1);
	}
	else if (i == 0 && path_command(cmd, g))
		return (1);
	return (0);
}

void	close_fds(t_command *cmd, int i)
{
	int	j;

	j = 0;
	if (i == -1)
	{
		while (cmd)
		{
			if (cmd->fd[1] != STDOUT_FILENO)
				close(cmd->fd[1]);
			if (cmd->fd[0] != STDIN_FILENO)
				close(cmd->fd[0]);
			cmd = cmd->next;
		}
		return ;
	}
	while (cmd && i > j)
	{
		if (cmd->fd[1] != STDOUT_FILENO)
			close (cmd->fd[1]);
		if (cmd->fd[0] != STDIN_FILENO)
			close(cmd->fd[0]);
		j++;
		cmd = cmd->next;
	}
}
