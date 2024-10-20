/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:42:53 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/20 21:49:02 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <readline/readline.h>

void	ctrl_c(int num)
{
    (void)num; // 'num' parametresini kullanmadığımızı belirtmek için

    // Yeni bir satır karakteri yazdır
    ft_putchar_fd('\n', 1);

    // Readline kütüphanesine yeni bir satırın başladığını bildir
    rl_on_new_line();

    // Mevcut satırı boş bir satırla değiştir
    rl_replace_line("", 0);

    // Global hata numarasını 128 + 'num' olarak ayarla
    _global(NULL)->error_no = 128 + num;

    // Readline kütüphanesini yeniden ekrana yazdır
    rl_redisplay();
}

void	parent_sigint2(int sig)
{
	(void)sig;
	ft_putchar_fd(('\n'), 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	heredoc_sig(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', 1);
	error_program(0, 128 + sig);
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
