/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:23:38 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/20 17:54:16 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_quotes_inplace(char **c_val)
{
    size_t	i;    // Döngü için indeks değişkeni
    size_t	j;    // Yeni string için indeks değişkeni
    int		in_s; // Tek tırnak içinde olup olmadığını belirten bayrak
    int		in_d; // Çift tırnak içinde olup olmadığını belirten bayrak

    // c_val işaretçisinin gösterdiği her string için döngü
    while (*c_val)
    {
        i = 0;    // Orijinal string için indeks sıfırla
        j = 0;    // Yeni string için indeks sıfırla
        in_s = 0; // Tek tırnak bayrağını sıfırla
        in_d = 0; // Çift tırnak bayrağını sıfırla

        // Stringin sonuna kadar döngü
        while ((*c_val)[i])
        {
            // Mevcut karakterin tırnak durumunu güncelle
            toggle_quote((*c_val)[i], &in_s, &in_d);

            // Eğer karakter tek tırnak ve çift tırnak içinde değilse veya
            // karakter çift tırnak ve tek tırnak içinde değilse
            if (((*c_val)[i] == '\'' && !in_d)
                || ((*c_val)[i] == '"' && !in_s))
                i++; // Karakteri atla
            else
                (*c_val)[j++] = (*c_val)[i++]; // Karakteri yeni stringe ekle
        }

        // Yeni stringin sonuna null karakter ekle
        (*c_val)[j] = '\0';

        // Bir sonraki stringe geç
        c_val++;
    }
}

static int	expand(t_global *g, t_command *clst)
{
    size_t	i; // Döngü için indeks değişkeni

    i = 0; // İndeksi sıfırla
    g->single_quotes = 0; // Tek tırnak sayacını sıfırla
    g->double_quotes = 0; // Çift tırnak sayacını sıfırla

    // Komutun değerleri boyunca döngü
    while (clst->value[i])
    {
        // Eğer değer içinde '$' karakteri varsa
        if (ft_strnstr(clst->value[i], "$", ft_strlen(clst->value[i])))
            // 'dollar' fonksiyonunu çağır ve eğer başarısız olursa 1 döndür
            if (!dollar(g, &clst->value[i]))
                return (1);

        // Eğer değer içinde '~' karakteri varsa
        if (ft_strnstr(clst->value[i], "~", ft_strlen(clst->value[i])))
            // 'home' fonksiyonunu çağır ve eğer başarısız olursa 1 döndür
            if (!home(g, &clst->value[i], 0, NULL))
                return (1);

        i++; // İndeksi artır
    }

    // Boş elemanları kaldır
    remove_empty_elements(clst->value); // values içindeki boş elemanları kaldır

    // Tırnakları yerinde kaldır
    remove_quotes_inplace(clst->value); // values içindeki tırnakları kaldır

    // Başarıyla tamamlandığında 0 döndür
    return (0);
}

static int	redirects(char **str, t_global *g, size_t *i)
{
	char	*tmp;

	if (ft_strnstr(str[*i + 1], "$", ft_strlen(str[*i + 1]))) // Eğer değer içinde '$' karakteri varsa açar
		dollar(g, &str[*i + 1]);
	if (ft_strnstr(str[*i + 1], "~", ft_strlen(str[*i + 1]))) // Eğer değer içinde '~' karakteri varsa açar
		home(g, &str[*i + 1], 0, NULL);
	if (!str[*i + 1][0])
		return (-1);
	tmp = quote_clean(str[*i + 1], 0, 0); // Tırnakları temizler
	str[*i + 1] = tmp; // temizlenmiş hali
	*i += 2;
	return (0);
}

static int	rdr(t_global *g, t_command *clst)
{
	size_t		i;
	int			res;

	i = 0;
	g->single_quotes = 0;
	g->double_quotes = 0;
	while (clst->rds && clst->rds[i] && clst->rds[i + 1])
	{
		res = redirects(clst->rds, g, &i); // yönlendirmelerinin düzgün olup olmadığını kontrol eder
		if (res == 1)
			return (1);
		if (res == -1)
		{
			clst->is_work = 0;
			ft_putendl_fd("minishell: unexpected redirect", 2);
			return (0);
		}
	}
	return (0);
}

void	expander(t_global *g)
{
    t_command	*cmd; // Komut listesi için işaretçi

    // Eğer kontrol değeri 0 ise, fonksiyondan çık
    if (g->control == 0)
        return ;

    // Komut listesinin başına işaretçi ata
    cmd = g->cmd_list;

    // Komut listesi boyunca döngü
    while (cmd)
    {
        // 'expand' fonksiyonunu çağır ve eğer başarısız olursa hata işle
        if (expand(g, cmd))
        {
            g->error_no = 12; // Hata numarasını 12 olarak ayarla
            error_program(0, 12); // Hata programını çağır
            return; // Fonksiyondan çık
        }

        // 'rdr' fonksiyonunu çağır ve eğer başarısız olursa hata işle
        if (rdr(g, cmd))
        {
            g->error_no = 12; // Hata numarasını 12 olarak ayarla
            error_program(0, 12); // Hata programını çağır
            return; // Fonksiyondan çık
        }

        // Bir sonraki komuta geç
        cmd = cmd->next;
    }
}
