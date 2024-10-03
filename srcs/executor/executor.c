/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:12:15 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/03 17:48:33 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	rdr_network(t_global *g, t_command **cmd)
{
	size_t	i;

	i = 0;
	while ((*cmd)->rds[i])
	{
		if (files(*cmd, &i, -1) == -1)
		{
			(*cmd)->is_work = 0;
			g->error_no = 1;
			*cmd = (*cmd)->next;
			return (0);
		}
		i++;
	}
	return (1);
}

static void	fd_config(t_global *g, t_command *cmd, int *i)
{
	int	fd[2];

	while (cmd)
	{
		if (cmd->p_type == 2)
		{
			if (pipe(fd) == -1)
				return (error_program(ERROR_PIPE, g->error_no));
			cmd->fd[1] = fd[1];
			cmd->next->fd[0] = fd[0];
		}
		(*i)++;
		if (cmd->is_work)
		{
			if (cmd->rds)
				if (!rdr_network(g, &cmd))
					continue ;
		}
		if (cmd)
			cmd = cmd->next;
	}
}

static void	execute_it(t_command *cmd, t_global *g, int i, int num)
{
	if (!num)
		cmd->pid = fork();
	if (cmd->pid == -1)
	{
		perror("FORK");
		g->error_no = 1;
		error_program(ERROR_FORK, g->error_no);
	}
	else if (cmd->pid == 0)
	{
		catch_signal(2);
		dup2(cmd->fd[1], STDOUT_FILENO);
		if (cmd->fd[1] != STDOUT_FILENO)
			close(cmd->fd[1]);
		dup2(cmd->fd[0], STDIN_FILENO);
		if (cmd->fd[0] != STDIN_FILENO)
			close(cmd->fd[0]);
		(void)i;
		execve(cmd->cmdpath, cmd->value, g->the_env);
		perror("EXECVE");
		g->error_no = 1;
		error_program("Error: Execute", g->error_no);
	}
	catch_signal(3);
}

void	run(t_global *g, t_command *cmd, int i, int num)
{
	if (num)
	{
		cmd->pid = fork();
		if (cmd->pid == -1)
		{
			perror("FORK");
			g->error_no = 1;
			error_program(0, g->error_no);
		}
		else if (cmd->pid != 0)
			return (catch_signal(3));
		catch_signal(2);
	}
	if (!builtin_check(cmd))
		;
	else if (is_command_ok(cmd, g))
		execute_it(cmd, g, i, num);
	else
	{
		if (num)
			error_program(ERROR_FORK, g->error_no);
	}
}

void	executor(t_global *g)
{
	t_command	*cmd;
	int			i;
	int			num;

	i = 0;
	num = 0;
	cmd = g->cmd_list;
	if (g->control == 0)
		return ;
	g->path = check_malloc_split(ft_split(env_finder("PATH")->value, ':'));
	fd_config(g, cmd, &i);
	if (cmd && cmd->next)
		num = 1;
	while (cmd)
	{
		if (cmd->is_work)
			run(g, cmd, i, num);
		if (cmd->fd[1] != STDOUT_FILENO)
			close(cmd->fd[1]);
		if (cmd->fd[0] != STDIN_FILENO)
			close(cmd->fd[0]);
		cmd = cmd->next;
	}
	wait_func(g, cmd);
	prepare_next(g);
}