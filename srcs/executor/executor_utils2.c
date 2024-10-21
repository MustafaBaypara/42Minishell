/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:34:36 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/21 11:53:34 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "sys/wait.h"
#include "fcntl.h"
#include <unistd.h>
#include <stdio.h>

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
    int	stat; // Çocuk sürecin durumunu tutmak için değişken

    // Komut listesinin sonuna kadar ilerle
    while (cmd && cmd->next)
        cmd = cmd->next;

    // Eğer komut geçerli ve çalıştırılabilir durumda ise
    if (cmd && cmd->is_work)
    {
        // Çocuk sürecin tamamlanmasını bekle
        waitpid(cmd->pid, &stat, 0);

        // Eğer çocuk süreç normal bir şekilde sonlandıysa
        if (cmd->pid != -1 && WIFEXITED(stat))
            // Çıkış durumunu global hata numarasına ata
            g->error_no = WEXITSTATUS(stat);

        // Eğer çocuk süreç bir sinyal ile sonlandıysa
        else if (cmd->pid != -1 && WIFSIGNALED(stat))
            // Sinyal numarasını global hata numarasına ata
            g->error_no = 128 + WTERMSIG(stat);
    }

    // Tüm çocuk süreçlerin tamamlanmasını bekle
    while (wait(NULL) != -1)
        ;
}

static int	check_file(t_command *cmd, int fd, int i)
{
    // Eğer dosya tanımlayıcısı geçersizse (i == -1), 0 döndür
    if (i == -1)
        return (0);

    // Eğer dosya tanımlayıcısı standart giriş ise (STDIN_FILENO)
    if (fd == STDIN_FILENO)
    {
        // Eğer komutun giriş dosya tanımlayıcısı standart giriş değilse, kapat
        if (cmd->fd[0] != STDIN_FILENO)
            close(cmd->fd[0]);

        // Komutun giriş dosya tanımlayıcısını yeni dosya tanımlayıcısı ile değiştir
        cmd->fd[0] = dup(i);
    }
    // Eğer dosya tanımlayıcısı standart çıkış ise (STDOUT_FILENO)
    else if (fd == STDOUT_FILENO)
    {
        // Eğer komutun çıkış dosya tanımlayıcısı standart çıkış değilse, kapat
        if (cmd->fd[1] != STDOUT_FILENO)
            close(cmd->fd[1]);

        // Komutun çıkış dosya tanımlayıcısını yeni dosya tanımlayıcısı ile değiştir
        cmd->fd[1] = dup(i);
    }

    // Başarıyla tamamlandığında 1 döndür
    return (1);
}

int	files(t_command *cmd, size_t *i, int fd)
{
    // Eğer yönlendirme ">>" ise (ekleme modu)
    if (!ft_strncmp(cmd->rds[*i], ">>", 2))
    {
        // Dosyayı ekleme modunda aç
        fd = open(cmd->rds[++(*i)], O_CREAT | O_APPEND | O_WRONLY, 0777);
        // Dosya kontrolü yap ve eğer başarısız olursa hata mesajı ver ve -1 döndür
        if (!check_file(cmd, STDOUT_FILENO, fd))
            return (perror(cmd->rds[*i]), -1);
    }
    // Eğer yönlendirme "<" ise (giriş dosyası)
    else if (!ft_strncmp(cmd->rds[*i], "<", ft_strlen(cmd->rds[*i])))
    {
        // Dosyayı okuma modunda aç
        fd = open(cmd->rds[++(*i)], O_RDONLY, 0777);
        // Dosya kontrolü yap ve eğer başarısız olursa hata mesajı ver ve -1 döndür
        if (!check_file(cmd, STDIN_FILENO, fd))
            return (perror(cmd->rds[*i]), -1);
    }
    // Eğer yönlendirme "<<" ise (heredoc)
    else if (!ft_strncmp(cmd->rds[*i], "<<", 2))
    {
        // İndeksi artır
        ++(*i);
        // Dosya kontrolü yap (heredoc dosya tanımlayıcısı kullanılır)
        check_file(cmd, STDIN_FILENO, cmd->the_fd);
    }
    // Eğer yönlendirme ">" ise (çıkış dosyası)
    else if (!ft_strncmp(cmd->rds[*i], ">", 1))
    {
        // Dosyayı yazma modunda aç (varsa içeriği sil)
        fd = open(cmd->rds[++(*i)], O_TRUNC | O_CREAT | O_WRONLY, 0777);
        // Dosya kontrolü yap ve eğer başarısız olursa hata mesajı ver ve -1 döndür
        if (!check_file(cmd, STDOUT_FILENO, fd))
            return (perror(cmd->rds[*i]), -1);
    }
    // Başarıyla tamamlandığında 0 döndür
    return (0);
}
