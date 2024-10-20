/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:50:22 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/20 17:43:36 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

static void	*heredoc_expander(char *s, t_global *g)
{
    size_t	i;      // Döngü için indeks değişkeni
    size_t	start;  // Alt dizinin başlangıç indeksini tutar
    char	*tmp;   // Geçici string değişkeni

    i = 0;
    tmp = check_malloc(ft_strdup("")); // Boş bir string oluştur ve tmp'ye ata

    while (s[i]) // Stringin sonuna kadar döngü
    {
        if (s[i] == '$') // Eğer karakter '$' ise
            tmp = dollar_sign(tmp, s, &i, g); // '$' işaretini genişlet ve tmp'ye ata
        else
        {
            start = i; // Alt dizinin başlangıç indeksini ayarla
            while (s[i] && s[i] != '$') // '$' işaretine kadar veya stringin sonuna kadar ilerle
                i++;
            tmp = check_malloc(ft_strjoin(tmp, check_malloc(ft_substr(s, start, i - start))));
        }
    }
    return (tmp); // Genişletilmiş stringi döndür
}

static void	on_heredoc(t_global *g, int *fd, char *d)
{
    char	*line;

    while (1)
    {
        line = (readline("> ")); // Kullanıcıdan giriş al
        if (!line || (!ft_strncmp(line, d, ft_strlen(line)) \
        && !ft_strncmp(line, d, ft_strlen(d))))
        {
            close(fd[1]); // Pipe'in yazma ucunu kapat
            g->error_no = 0;
            error_program(0, g->error_no); // Hata programını çağır
        }
        line = check_malloc(line); // Bellek kontrolü yap
        line = heredoc_expander(line, g); // Giriş satırını genişlet
        ft_putendl_fd(line, fd[1]); // Satırı pipe'e yaz
        rl_clear_history(); // Geçmişi temizle
    }
}

static int	heredoc_wait(t_global *g, t_command *cmd)
{
    int	i;

    // Çocuk sürecin tamamlanmasını bekle
    waitpid(cmd->pid, &i, 0);

    // Eğer çocuk süreç normal bir şekilde sonlandıysa
    if (WIFEXITED(i)) // normal bir şekilde dönüp dönmediğini kontrol eder
    {
        // Çıkış durumunu al ve g->error_no'ya ata
        g->error_no = WEXITSTATUS(i); // normal döndüyse çıkış durumunu alır

        // Eğer çıkış durumu 1 ise, SIGINT döndür
        if (g->error_no == 1)
            return (g->error_no = 1, SIGINT);
        // Eğer çıkış durumu 12 ise, bellek hatası programını çağır
        else if (g->error_no == 12)
            error_program(ERROR_MALLOC, 12);
        // Diğer durumlarda, çıkış durumunu g->error_no'ya ata
        else
            g->error_no = WEXITSTATUS(i);
    }

    // Tüm çocuk süreçlerin tamamlanmasını bekle
    while (wait(NULL) != -1)
        ;

    // Fonksiyon başarılı bir şekilde tamamlandığında 0 döner
    return (0);
}

static int	loop_heredoc(t_global *g, int *fd, t_command *cmd, char *d)
{
    // Yeni bir süreç oluşturmak için fork() kullan
    cmd->pid = fork();
    
    // Eğer fork() başarısız olursa
    if (cmd->pid == -1)
        return (g->error_no = 1, error_program(ERROR_FORK, 1), 0); // Hata numarasını 1 yap ve hata programını çağır
    
    // Eğer çocuk süreçte isek
    else if (cmd->pid == 0)
    {
        catch_signal(4); // ctrlc gelirse newline ve process'i kapat
        close(fd[0]); // Pipe'in okuma ucunu kapat
        on_heredoc(g, fd, d); // Heredoc işlemini gerçekleştir
    }
    
    // Eğer ana süreçte isek
    else
    {
        close(fd[1]); // Pipe'in yazma ucunu kapat
        if (heredoc_wait(g, cmd) == SIGINT) // Heredoc işleminin tamamlanmasını bekle
            return (close(fd[0]), SIGINT); // Eğer SIGINT sinyali alınırsa, pipe'in okuma ucunu kapat ve SIGINT döndür
        cmd->the_fd = dup(fd[0]); // Pipe'in okuma ucunun bir kopyasını oluştur ve cmd->the_fd'ye ata
        close(fd[0]); // Pipe'in okuma ucunu kapat
    }
    
    // Fonksiyon başarılı bir şekilde tamamlandığında 1 döner
    return (1);
}

int heredocs(t_global *g, t_command *cmd)
{
    int fd[2]; // Pipe için dosya tanımlayıcıları
    int i;

    // Eğer heredoc kontrolü başarısız olursa, fonksiyon 1 döner
    if (check_hdoc(cmd) == 0)
        return (1);

    // Komut listesi boyunca döngü
    while (cmd)
    {
        i = -1;
        // Komutun yönlendirmeleri boyunca döngü
        while (cmd->rds && cmd->rds[++i])
        {
            // Eğer yönlendirme "<<" ile başlıyorsa
            if (!ft_strncmp(cmd->rds[i], "<<", 2))
            {
                // Eğer daha önce bir dosya tanımlayıcı açıksa, kapat
                if (cmd->the_fd != -1)
                    close(cmd->the_fd);

                // Pipe oluştur ve hata kontrolü yap
                if (pipe(fd) == -1 || i++ == -1)
                    return (error_program(ERROR_PIPE, 1), 0);

                // Yönlendirmeyi tırnaklardan temizle
                cmd->rds[i] = quote_clean(cmd->rds[i], 0, 0);

                // Heredoc döngüsünü çalıştır ve sinyal kontrolü yap
                if (loop_heredoc(g, fd, cmd, cmd->rds[i]) == SIGINT)
                    return (catch_signal(1), 0);
            }
        }
        // Komutun pid'sini -1 yap
        cmd->pid = -1;
        // Bir sonraki komuta geç
        cmd = cmd->next;
    }
    // Sinyal yakalama ve fonksiyonu başarıyla tamamlama
    return (catch_signal(1), 1);
}
