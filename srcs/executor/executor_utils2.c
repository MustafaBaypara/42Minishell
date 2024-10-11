/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:34:36 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/02 17:49:35 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "sys/wait.h"
#include "fcntl.h"

int	check_slash(char *val)
{
	size_t	i;

	i = -1;
	while (val[++i])
		if (val[i] == '/')
			return (1);
	return (0);
}

void	catch_error(t_command *cmd, int i, t_global *g)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->value[0], 2);
	if (i == 1)
	{
		ft_putendl_fd(": command not found", 2);
		g->error_no = 127;
	}
	else if (i == 2)
	{
		ft_putendl_fd(": No such file or directory", 2);
		g->error_no = 127;
	}
	else if (i == 3)
	{
		ft_putendl_fd(": Permission denied", 2);
		g->error_no = 126;
	}
	else if (i == 4)
	{
		ft_putendl_fd(": Is a directory", 2);
		g->error_no = 126;
	}
}

void	wait_func(t_global *g, t_command *cmd)
{
	int	stat;

	while (cmd && cmd->next)
		cmd = cmd->next;
	if (cmd && cmd->is_work)
	{
		waitpid(cmd->pid, &stat, 0);
		if (cmd->pid != -1 && WIFEXITED(stat))
			g->error_no = WEXITSTATUS(stat);
		else if (cmd->pid != -1 && WIFSIGNALED(stat))
			g->error_no = 128 + WTERMSIG(stat);
	}
	while (wait(NULL) != -1)
		;
}

static int	check_file(t_command *cmd, int fd, int i)
{
	if (i == -1)
		return (0);
	if (fd == STDIN_FILENO)
	{
		if (cmd->fd[0] != STDIN_FILENO)
			close(cmd->fd[0]);
		cmd->fd[0] = i;
	}
	else if (fd == STDOUT_FILENO)
	{
		if (cmd->fd[1] != STDOUT_FILENO)
			close(cmd->fd[1]);
		cmd->fd[1] = i;
	}
	return (1);
}

int	files(t_command *cmd, size_t *i, int fd)
{
	if (!ft_strncmp(cmd->rds[*i], ">>", 2))
	{
		fd = open(cmd->rds[++(*i)], O_CREAT | O_APPEND | O_WRONLY, 0777);
		if (!check_file(cmd, STDOUT_FILENO, fd))
			return (perror(cmd->rds[*i]), -1);
	}
	else if (!ft_strncmp(cmd->rds[*i], "<", ft_strlen(cmd->rds[*i])))
	{
		fd = open(cmd->rds[++(*i)], O_RDONLY, 0777);
		if (!check_file(cmd, STDIN_FILENO, fd))
			return (perror(cmd->rds[*i]), -1);
	}
	else if (!ft_strncmp(cmd->rds[*i], "<<", 2))
	{
		++(*i);
		check_file(cmd, STDIN_FILENO, cmd->the_fd);
	}
	else if (!ft_strncmp(cmd->rds[*i], ">", 1))
	{
		fd = open(cmd->rds[++(*i)], O_TRUNC | O_CREAT | O_WRONLY, 0777);
		if (!check_file(cmd, STDOUT_FILENO, fd))
			return (perror(cmd->rds[*i]), -1);
	}
	return (0);
}
