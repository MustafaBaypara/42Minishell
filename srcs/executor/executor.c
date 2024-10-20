/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:12:15 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/20 21:48:19 by mbaypara         ###   ########.fr       */
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
    // Eğer num 0 ise (yani birden fazla komut varsa)
    if (!num)
        // Yeni bir süreç oluşturmak için fork() kullan
        cmd->pid = fork();

    // Eğer fork() başarısız olursa
    if (cmd->pid == -1)
    {
        perror("FORK"); // Hata mesajı yazdır
        g->error_no = 1; // Hata numarasını 1 olarak ayarla
        error_program(ERROR_FORK, g->error_no); // Hata programını çağır
    }
    // Eğer çocuk süreçte isek
    else if (cmd->pid == 0)
    {
        catch_signal(2); // Çocuk süreçte sinyal yakalama fonksiyonunu çağır

        // Çıkış dosya tanımlayıcısını standart çıkışa yönlendir
        dup2(cmd->fd[1], STDOUT_FILENO);
        // Eğer çıkış dosya tanımlayıcısı standart çıkış değilse, kapat
        if (cmd->fd[1] != STDOUT_FILENO)
            close(cmd->fd[1]);

        // Giriş dosya tanımlayıcısını standart girişe yönlendir
        dup2(cmd->fd[0], STDIN_FILENO);
        // Eğer giriş dosya tanımlayıcısı standart giriş değilse, kapat
        if (cmd->fd[0] != STDIN_FILENO)
            close(cmd->fd[0]);

        // Diğer dosya tanımlayıcılarını kapat
        close_fds(cmd->next, i);

        // Komutu çalıştır
        execve(cmd->cmdpath, cmd->value, g->the_env);

        // Eğer execve başarısız olursa hata programını çağır
        error_program(0, g->error_no);
    }

    // Ana süreçte sinyal yakalama fonksiyonunu çağır
    catch_signal(3); // ctrl c de readline girdi promtunu temizler
}

void	run(t_global *g, t_command *cmd, int i, int num)
{
    // Eğer num 0'dan farklıysa (yani birden fazla komut varsa)
    if (num)
    {
        // Yeni bir süreç oluşturmak için fork() kullan
        cmd->pid = fork();
        // Eğer fork() başarısız olursa
        if (cmd->pid == -1)
        {
            perror("FORK"); // Hata mesajı yazdır
            g->error_no = 1; // Hata numarasını 1 olarak ayarla
            error_program(0, g->error_no); // Hata programını çağır
        }
        // Eğer çocuk süreçte değilsek (ana süreçteysek)
        else if (cmd->pid != 0)
            return (catch_signal(3)); // Sinyal yakalama fonksiyonunu çağır ve fonksiyondan çık
        catch_signal(2); // Çocuk süreçte sinyal yakalama fonksiyonunu çağır
    }
    // çocuk process işler 
    
    // Eğer komut bir yerleşik komut değilse
    if (!builtin_check(cmd, num))
        return ; // Fonksiyondan çık
    // Eğer komut yerleşik bir komut ise ve komut geçerliyse
    else if (is_command_ok(cmd, g)) // komutun yolunu bulup kontrol eder
        execute_it(cmd, g, i, num); // Komutu çalıştır
    else
    {
        // Eğer num 0'dan farklıysa (yani birden fazla komut varsa)
        if (num)
            error_program(0, g->error_no); // Hata programını çağır
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

    // single multiple komutları kontrol eder
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
