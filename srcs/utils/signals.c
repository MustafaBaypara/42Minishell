/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:42:53 by mbaypara          #+#    #+#             */
/*   Updated: 2024/09/27 14:01:06 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <signal.h>
#include <sys/signal.h>
#include <readline/readline.h>
#include <unistd.h>
#include <termios.h>

void	ctrl_c(int num)
{
	(void)num;
	ft_putchar_fd('\n', 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	parent_sigint2(int sig)
{
	(void)sig;

	ft_putchar_fd(('\n'), 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	echo_check(void)
{
	struct termios	tty;

	if (!tcgetattr(STDIN_FILENO, &tty))
	{
		tty.c_lflag &= ~ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &tty);
	}
}

void	heredoc_sig(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', 1);
	_global(NULL)->error_no = 1;
	error_program("", 1);
}

void	catch_signal(int num)
{
	if (num == 1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, ctrl_c);
	}
	else if (num == 2)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
	}
	else if (num == 3)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, parent_sigint2);
	}
	else if (num == 4)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, heredoc_sig);
	}
	else if (num == 5)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, SIG_IGN);
	}
}
