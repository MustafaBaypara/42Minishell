/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:42:53 by mbaypara          #+#    #+#             */
/*   Updated: 2024/09/25 17:59:32 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <signal.h>
#include <sys/signal.h>
#include <readline/readline.h>

void	new_area(int num)
{
	(void)num;
	ft_putchar_fd('\n', 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	

void	catch_signal(int num)
{
	if (num == 1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, new_area);
	}
	else if (num == 2)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
	}
	else if (num == 3)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, )
	}
}
