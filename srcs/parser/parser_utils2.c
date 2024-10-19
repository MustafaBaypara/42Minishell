/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:04:19 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/19 17:45:48 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_rdr(char *strs)
{
    int	i;    // Döngü sayacı
    int	in_s; // Tek tırnak içinde olup olmadığını belirten bayrak
    int	in_d; // Çift tırnak içinde olup olmadığını belirten bayrak

    i = 0;
    in_s = 0;
    in_d = 0;
    while (strs[i])  // Stringin sonuna kadar döngü
    {
        toggle_quote(strs[i], &in_s, &in_d);  // Tırnak işaretlerini kontrol et ve güncelle
        if (!in_s && !in_d)  // Eğer tırnak işaretleri içinde değilse
        {
            if (strs[i] == '<')  // Eğer karakter '<' ise
                return (1);  // 1 döndür (yönlendirme operatörü bulundu)
            if (strs[i] == '>')  // Eğer karakter '>' ise
                return (1);  // 1 döndür (yönlendirme operatörü bulundu)
        }
        i++;  // tırnak dışında yönlendirme varsa indeksi artır
    }
    return (0);  // Yönlendirme operatörü bulunamadıysa 0 döndür
}

static void	fill_rds(t_command *cmd, char **tmp, size_t *f)
{
    size_t	i;    // Döngü sayacı
    size_t	j;    // Yönlendirme operatörleri için sayacı
    char	**val;  // Komutun değerlerini tutan dizi

    i = 0;
    j = 0;
    val = cmd->value;  // Komutun değerlerini al
    while (val[i])  // Değerlerin sonuna kadar döngü
    {
        if (is_rdr(val[i]))  // Eğer değer bir yönlendirme operatörü ise
        {
            cmd->rds[j++] = val[i++];  // Yönlendirme operatörünü komutun rds dizisine ekle
            cmd->rds[j++] = val[i];  // Yönlendirme operatörünün hedefini komutun rds dizisine ekle
        }
        else
            tmp[(*f)++] = val[i];  // Değer yönlendirme operatörü değilse, tmp dizisine ekle
        i++;
    }
    tmp[*f] = NULL;  // tmp dizisinin sonuna NULL ekle
}

static size_t	rdr_count(char **str)
{
    size_t	len;  // Yönlendirme operatörlerinin sayısını tutar
    size_t	i;    // Döngü sayacı

    i = 0;
    len = 0;
    while (str[i])  // Dizi sonuna kadar döngü
    {
        if (!ft_strncmp(str[i], "<", 1) && ft_strlen(str[i]) == 1 && ++i)  // Eğer "<" operatörü varsa
            len++;
        else if (!ft_strncmp(str[i], ">", 1) && ft_strlen(str[i]) == 1 && ++i)  // Eğer ">" operatörü varsa
            len++;
        else if (!ft_strncmp(str[i], ">>", 2) && ft_strlen(str[i]) == 2 && ++i)  // Eğer ">>" operatörü varsa
            len++;
        else if (!ft_strncmp(str[i], "<<", 2) && ft_strlen(str[i]) == 2 && ++i)  // Eğer "<<" operatörü varsa
            len++;
        i++;
    }
    len *= 2;  // Yönlendirme operatörlerinin sayısını ikiyle çarp çünkü bağlı olan elemanı da saymak gerekiyor örn: ">> file"
    return (len);  // Yönlendirme operatörlerinin toplam sayısını döndür
}

int	rdr_position(t_command *cmds)
{
    char	**tmp;  // Geçici dizi
    char	**val;  // Komutun değerlerini tutan dizi
    size_t	i;      // Döngü sayacı
    size_t	j;      // Geçici dizi için sayacı

    i = 0;
    j = 0;
    val = cmds->value;  // Komutun değerlerini al
    if (rdr_count(val) <= 0) // Eğer yönlendirme operatörü yoksa
        return (1);  // 1 döndür (başarılı)
    while (val[i])  // Değerlerin sonuna kadar döngü
        i++;
    tmp = check_malloc(ft_calloc(i - rdr_count(val) + 1, sizeof(char *)));  // Geçici dizi için bellek ayır
    cmds->rds = check_malloc(ft_calloc(rdr_count(val) + 1, sizeof(char *)));  // Yönlendirme operatörleri için bellek ayır
    fill_rds(cmds, tmp, &j);  // Yönlendirme operatörlerini ve diğer değerleri ayır
    cmds->value = tmp;  // Komutun değerlerini güncelle
    return (1);  // 1 döndür (başarılı)
}
