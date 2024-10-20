/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:27:24 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/20 14:06:02 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_dsign(char *tmp)
{
    t_global	*g;   // Global yapı için bir işaretçi
    char		*num; // Hata numarasını tutacak bir string

    // Global yapıyı al
    g = _global(NULL);

    // Hata numarasını stringe çevir ve bellek tahsisini kontrol et
    num = check_malloc(ft_itoa(g->error_no));

    // Mevcut string ile hata numarasını birleştir ve bellek tahsisini kontrol et
    tmp = check_malloc(ft_strjoin(tmp, num));

    // Birleştirilmiş stringi döndür
    return (tmp);
}

static char	*expand_var(char *tmp, char *t_val, size_t *i)
{
    size_t	j;        // Döngü için indeks değişkeni
    char	*new_tmp; // Geçici string değişkeni
    t_env	*e_v;     // Ortam değişkeni için işaretçi

    j = 0;
    (*i)++; // İndeksi bir artır

    // Eğer karakter bir rakam, çift tırnak veya tek tırnak ise
    if (ft_isdigit(t_val[*i]) || t_val[*i + 1] == '"' || t_val[*i + 1] == '\'')
        return ((*i)++, tmp); // İndeksi bir artır ve tmp'yi döndür

    // Alfanümerik karakterler veya '_' karakteri boyunca döngü
    while ((ft_isalnum(t_val[*i + j]) || t_val[*i + j] == '_'))
        j++;

    // Alt diziyi oluştur ve bellek tahsisini kontrol et
    new_tmp = check_malloc(ft_strndup(&t_val[*i], j)); // $HOME komutundaki HOME KISMINI AYIRIR

    // Ortam değişkenini bul
    e_v = env_finder(new_tmp); // ENV İÇİNDE HOME ARATIR

    // Eğer ortam değişkeni bulunursa, değeri tmp'ye ekle
    if (e_v)
        tmp = check_malloc(ft_strjoin(tmp, e_v->value)); // VARSA VALUE kısmına home açar ve ekler

    // İndeksi j kadar artır
    *i += j;

    // Genişletilmiş stringi döndür
    return (tmp);
}

char	*dollar_sign(char *tmp, char *t_val, size_t *i, t_global *g)
{
    // Eğer '$' işaretinden sonra '?' geliyorsa
    if (t_val[*i + 1] == '?')
    {
        tmp = expand_dsign(tmp); // '$?' işaretini genişlet
        *i += 2; // İndeksi 2 artır
    }
    // Eğer '$' işaretinden sonra stringin sonu geliyorsa
    else if (t_val[*i + 1] == '\0')
    {
        tmp = check_malloc(ft_strjoin(tmp, "$")); // '$' işaretini tmp'ye ekle
        (*i)++; // İndeksi 1 artır
    }
    // Eğer '$' işaretinden sonra alfanümerik bir karakter, '_' veya çift tırnak, tek tırnak geliyorsa
    else if (ft_isalnum(t_val[*i + 1]) || t_val[*i + 1] == '_' \
    || ((t_val[*i + 1] == '"' || t_val[*i + 1] == '\'') \
    && !g->single_quotes && !g->double_quotes))
        tmp = expand_var(tmp, t_val, i); // Değişkeni genişlet
    else
    {
        tmp = check_malloc(ft_strjoin(tmp, "$")); // '$' işaretini tmp'ye ekle
        (*i)++; // İndeksi 1 artır
    }
    return (tmp); // Genişletilmiş stringi döndür
}
