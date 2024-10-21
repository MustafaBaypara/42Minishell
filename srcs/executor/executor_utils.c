/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:35:13 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/21 15:02:21 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>	
static int	check_path(t_command *cmd, t_global *g, int *i)
{
    struct stat	buf; // Dosya bilgilerini tutmak için stat yapısı

    // Eğer komutun değeri bir slash (/) içeriyorsa
    if (!check_slash(cmd->value[0]))
        return (0); // 0 döndür
    // Dosya bilgilerini almak için stat fonksiyonunu kullan
    if (stat(cmd->value[0], &buf) == 0)
    {
        // Eğer dosya bir dizin ise
        if (S_ISDIR(buf.st_mode))
            return (catch_error(cmd, 4, g), *i = 1, 0); // Hata yakala, i'yi 1 yap ve 0 döndür

        // Eğer dosya çalıştırılabilir değilse
        if (access(cmd->value[0], X_OK) != 0)
            return (catch_error(cmd, 3, g), *i = 1, 0); // Hata yakala, i'yi 1 yap ve 0 döndür

        return (1); // Dosya geçerli, 1 döndür
    }
    else
    {
        catch_error(cmd, 2, g); // Hata yakala
        *i = 1; // i'yi 1 yap
    }
    return (0); // 0 döndür
}

static int	check_dir(char *path, t_global *g, t_command *cmd)
{
    struct stat	buf; // Dosya bilgilerini tutmak için stat yapısı

    // Dosya bilgilerini almak için stat fonksiyonunu kullan
    if (stat(path, &buf) == 0)
    {
        // Eğer dosya bir dizin ise
        if (S_ISDIR(buf.st_mode))
        {
            // Hata yakala ve 1 döndür
            return (catch_error(cmd, 1, g), 1);
        }
    }

    // Eğer dosya bir dizin değilse veya stat fonksiyonu başarısız olursa 0 döndür
    return (0);
}

static int	path_command(t_command *cmd, t_global *g)
{
    char	*tmp; // Geçici string değişkeni
    size_t	i;    // Döngü için indeks değişkeni

    i = 0; // İndeksi sıfırla
    // Komutun değerini bir slash (/) ile birleştir ve tmp'ye ata
    tmp = check_malloc(ft_strjoin("/", cmd->value[0]));
    // Path dizisi boyunca döngü
    while (g->path[i])
    {
        // Path'in mevcut elemanını tmp ile birleştir ve cmd->cmdpath'e ata
        cmd->cmdpath = check_malloc(ft_strjoin(g->path[i], tmp));
        // Eğer dizin kontrolü başarılı olursa
        if (check_dir(cmd->cmdpath, g, cmd)) // birebir aynı olması için ypaılmış X11 çalıştığında usr/bin/X11 bulursa burdan çıkıyor
            // cmd->cmdpath'i NULL yap ve 0 döndür
            return (cmd->cmdpath = NULL, 0);

        // Eğer dosya çalıştırılabilir ise
        if (access(cmd->cmdpath, X_OK) == 0) // cat yazıldığında bin/cat içinde buluyor ve çalışıp çalışmadığını kontrol ediyor
            return (1); // 1 döndür

        // EĞER YUKARDAKİLER OLMADIYSA YOKTUR DEMEK VE CMD KAPATILIYOR VE Garbage listeden cmd->cmdpath'i kaldır
        remove_from_list(&g->garbage_list, cmd->cmdpath); // 

        // cmd->cmdpath'i NULL yap
        cmd->cmdpath = NULL;

        // İndeksi artır
        i++;
    }

    // Hata yakala ve 0 döndür
    return (catch_error(cmd, 1, g), 0); // KOMUT HİÇ BİR YERDE YOKSA hatayı verir ve 0 döndürür
}

int	is_command_ok(t_command *cmd, t_global *g)
{
    int	i; // Döngü ve kontrol için indeks değişkeni

    i = 0; // İndeksi sıfırla

    // Eğer komut geçerli değilse veya komutun değeri boşsa, 0 döndür
    if (cmd && (!cmd->value[0] || !cmd->value[0][0]))
        return (0);

    // sadece o dizindeki ./a.out gibi komutları kontrol eder
    if (check_path(cmd, g, &i))
    {
        // Komutun path'ini kopyala ve cmd->cmdpath'e ata
        cmd->cmdpath = check_malloc(ft_strdup(cmd->value[0]));
        return (1); // Komut geçerli, 1 döndür
    }
    // Eğer path kontrolü başarısız olursa ve i 0 ise
    else if (i == 0 && path_command(cmd, g))
        return (1); // Komut geçerli, 1 döndür

    // Komut geçerli değilse, 0 döndür
    return (0);
}

void	close_fds(t_command *cmd, int i)
{
    int	j; // Döngü için indeks değişkeni

    j = 0; // İndeksi sıfırla
    if (i == -1) // Eğer i -1 ise, tüm dosya tanımlayıcılarını kapat
    {
        while (cmd) // Komut listesi boyunca döngü
        {
            if (cmd->fd[1] != STDOUT_FILENO) // Eğer çıkış dosya tanımlayıcısı standart çıkış değilse
                close(cmd->fd[1]); // Çıkış dosya tanımlayıcısını kapat
            if (cmd->fd[0] != STDIN_FILENO) // Eğer giriş dosya tanımlayıcısı standart giriş değilse
                close(cmd->fd[0]); // Giriş dosya tanımlayıcısını kapat
            cmd = cmd->next; // Bir sonraki komuta geç
        }
        return ; // Fonksiyondan çık
    }
    while (cmd && i > j) // Komut listesi boyunca ve j, i'den küçük olduğu sürece döngü
    {
        if (cmd->fd[1] != STDOUT_FILENO) // Eğer çıkış dosya tanımlayıcısı standart çıkış değilse
            close(cmd->fd[1]); // Çıkış dosya tanımlayıcısını kapat
        if (cmd->fd[0] != STDIN_FILENO) // Eğer giriş dosya tanımlayıcısı standart giriş değilse
            close(cmd->fd[0]); // Giriş dosya tanımlayıcısını kapat
        j++; // İndeksi artır
        cmd = cmd->next; // Bir sonraki komuta geç
    }
}
