/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:50:22 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/21 16:13:43 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

static void	*heredoc_expander(char *s, t_global *g)
{
	size_t	i;
	size_t	start;
	char	*tmp;

	i = 0;
	tmp = check_malloc(ft_strdup(""));
	while (s[i])
	{
		if (s[i] == '$')
			tmp = dollar_sign(tmp, s, &i, g);
		else
		{
			start = i;
			while (s[i] && s[i] != '$')
				i++;
			tmp = check_malloc(ft_strjoin(tmp, \
			check_malloc(ft_substr(s, start, i))));
		}
	}
	return (tmp);
}

static void	on_heredoc(t_global *g, int *fd, char *d)
{
	char	*line;

	while (1)
	{
		line = (readline("> "));
		if (!line || (!ft_strncmp(line, d, ft_strlen(line)) \
		&& !ft_strncmp(line, d, ft_strlen(d))))
		{
			close(fd[1]);
			g->error_no = 0;
			error_program(0, g->error_no);
		}
		line = check_malloc(line);
		line = heredoc_expander(line, g);
		ft_putendl_fd(line, fd[1]);
		rl_clear_history();
	}
}

static int	heredoc_wait(t_global *g, t_command *cmd)
{
	int	i;

	waitpid(cmd->pid, &i, 0);
	if (WIFEXITED(i))
	{
		g->error_no = WEXITSTATUS(i);
		if (g->error_no == 1)
			return (g->error_no = 1, SIGINT);
		else if (g->error_no == 12)
			error_program(ERROR_MALLOC, 12);
		else
			g->error_no = WEXITSTATUS(i);
	}
	while (wait(NULL) != -1)
		;
	return (0);
}

static int	loop_heredoc(t_global *g, int *fd, t_command *cmd, char *d)
{
	cmd->pid = fork();
	if (cmd->pid == -1)
		return (g->error_no = 1, error_program(ERROR_FORK, 1), 0);
	else if (cmd->pid == 0)
	{
		catch_signal(4);
		close(fd[0]);
		on_heredoc(g, fd, d);
	}
	else
	{
		close(fd[1]);
		if (heredoc_wait(g, cmd) == SIGINT)
			return (close(fd[0]), SIGINT);
		cmd->the_fd = dup(fd[0]);
		close(fd[0]);
	}
	return (1);
}

int	heredocs(t_global *g, t_command *cmd)
{
	int	fd[2];
	int	i;

	if (check_hdoc(cmd) == 0)
		return (1);
	while (cmd)
	{
		i = -1;
		while (cmd->rds && cmd->rds[++i])
		{
			if (!ft_strncmp(cmd->rds[i], "<<", 2))
			{
				if (cmd->the_fd != -1)
					close(cmd->the_fd);
				if (pipe(fd) == -1 || i++ == -1)
					return (error_program(ERROR_PIPE, 1), 0);
				cmd->rds[i] = quote_clean(cmd->rds[i], 0, 0);
				if (loop_heredoc(g, fd, cmd, cmd->rds[i]) == SIGINT)
					return (catch_signal(1), 0);
			}
		}
		cmd->pid = -1;
		cmd = cmd->next;
	}
	return (catch_signal(1), 1);
}
