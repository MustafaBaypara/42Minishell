/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:12:15 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/20 19:12:28 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

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
    int	fd[2]; // Pipe için dosya tanımlayıcıları

    // Eğer "PATH" ortam değişkeni varsa, onu kullanarak path'i ayarla
    if (env_finder("PATH"))
        g->path = check_malloc_split(ft_split(env_finder("PATH")->value, ':'));
    else
        // Eğer "PATH" ortam değişkeni yoksa, "PWD" ortam değişkenini kullanarak path'i ayarla
        g->path = check_malloc_split(ft_split(env_finder("PWD")->value, ':'));

    // Komut listesi boyunca döngü
    while (cmd)
    {
        // Eğer komutun pipe türü 2 ise
        if (cmd->p_type == 2)
        {
            // Pipe oluştur ve hata kontrolü yap
            if (pipe(fd) == -1)
                return (error_program(ERROR_PIPE, 1));

            // Komutun çıkış dosya tanımlayıcısını pipe'in yazma ucuna ayarla
            cmd->fd[1] = fd[1];

            // Bir sonraki komutun giriş dosya tanımlayıcısını pipe'in okuma ucuna ayarla
            cmd->next->fd[0] = fd[0];
        }
        // İndeksi artır
        (*i)++;
        // dosya kontorllerinde is work izinini kontrol eder
        if (cmd->is_work)
        {
            // Eğer komutun yönlendirmeleri varsa
            if (cmd->rds)
                // Yönlendirme ağını kontrol et ve eğer başarısız olursa döngüye devam et
                if (!rdr_network(g, &cmd))
                    continue ;
        }

        // Bir sonraki komuta geç
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
		close_fds(cmd->next, i);
		execve(cmd->cmdpath, cmd->value, g->the_env);
		error_program(0, g->error_no);
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
	if (!builtin_check(cmd, num))
		return ;
	else if (is_command_ok(cmd, g))
		execute_it(cmd, g, i, num);
	else
	{
		if (num)
			error_program(0, g->error_no);
	}
}

void	executor(t_global *g, int i, int num)
{
    t_command	*cmd; // Komut listesi için işaretçi

    // Eğer kontrol değeri 0 ise, fonksiyondan çık
    if (g->control == 0)
        return ;

    // Komut listesinin başına işaretçi ata
    cmd = g->cmd_list;

    // Dosya tanımlayıcılarını yapılandır
    fd_config(g, cmd, &i);

    // Eğer komut listesi varsa ve bir sonraki komut varsa, num'u 1 yap
    if (cmd && cmd->next)
        num = 1;

    // Komut listesi boyunca döngü
    while (cmd)
    {
        // Eğer komut çalıştırılabilir durumda ise
        if (cmd->is_work)
            // Komutu çalıştır
            run(g, cmd, i, num);

        // Eğer komutun çıkış dosya tanımlayıcısı standart çıkış değilse, kapat
        if (cmd->fd[1] != STDOUT_FILENO)
            close(cmd->fd[1]);

        // Eğer komutun giriş dosya tanımlayıcısı standart giriş değilse, kapat
        if (cmd->fd[0] != STDIN_FILENO)
            close(cmd->fd[0]);

        // Eğer komutun dosya tanımlayıcısı geçerli bir dosya tanımlayıcısı ise, kapat
        if (cmd->the_fd >= STDIN_FILENO)
            close(cmd->the_fd);

        // Bir sonraki komuta geç
        cmd = cmd->next;
    }

    // Tüm komutların tamamlanmasını bekle
    wait_func(g, g->cmd_list);

    // Bir sonraki komut için hazırlık yap
    prepare_next(g);
}
