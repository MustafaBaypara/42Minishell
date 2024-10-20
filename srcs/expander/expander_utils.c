/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 18:09:07 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/20 15:46:22 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_empty_elements(char **arr)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		if (arr[i] != NULL && ft_strncmp(arr[i], "", 1) != 0)
			arr[j++] = arr[i];
		i++;
	}
	arr[j] = NULL;
}

int	home(t_global *g, char **value, size_t i, char *tmp)
{
    char	*str; // Orijinal stringi tutmak için bir işaretçi

    // Boş bir string oluştur ve tmp'ye ata
    tmp = check_malloc(ft_strdup(""));

    // value işaretçisinin gösterdiği stringi str'ye ata
    str = *value;

    // Stringin sonuna kadar döngü
    while (str[i])
    {
        // Eğer karakter '~' ise ve tek tırnak veya çift tırnak içinde değilse
        if (str[i] == '~' && !g->single_quotes && !g->double_quotes)
        {
            // Eğer '~' karakteri öncesinde boşluk olmayan bir karakter varsa veya
            // '~' karakteri sonrasında boşluk olmayan bir karakter ve '/' karakteri yoksa
            if ((i > 0 && !is_white_space((str[i - 1])))
                || (!is_white_space(str[i + 1]) && str[i + 1] != '\0'
                    && str[i + 1] != '/'))
                // '~' karakterini tmp'ye ekle
                tmp = check_malloc(ft_strjoin(tmp, "~"));
            else
            {
                // Eğer "HOME" ortam değişkeni bulunamazsa, 1 döndür
                if (!env_finder("HOME"))
                    return (1);

                // "HOME" ortam değişkeninin değerini tmp'ye ekle
                tmp = check_malloc(ft_strjoin(tmp, env_finder("HOME")->value));
            }
        }
        else
            // Mevcut karakteri tmp'ye ekle
            tmp = check_malloc(ft_strappend(tmp, &str[i], 1));

        // Tek tırnak ve çift tırnak durumlarını güncelle
        toggle_quote(str[i], &g->single_quotes, &g->double_quotes);

        // İndeksi artır
        i++;
    }

    // value işaretçisinin gösterdiği değeri tmp olarak güncelle ve 1 döndür
    return (*value = tmp, 1);
}

char	*append_literal(char *tmp, char *token_value, size_t *start, size_t *i)
{
	tmp = ft_strappend(tmp, &token_value[*start], *i - *start);
	return (tmp);
}

int	dollar(t_global	*g, char **value)
{
	char	*tmp;
	size_t	i;
	size_t	start;

	i = 0;
	tmp = check_malloc(ft_strdup(""));
	while ((*value)[i])
	{
		if ((*value)[i] == '$' && !g->single_quotes) //dolar varsa ve tek tırnak içinde değilse
			tmp = dollar_sign(tmp, (*value), &i, g);
		else
		{
			start = i;
			while ((*value)[i] && !((*value)[i] == '$' && !g->single_quotes))
				toggle_quote((*value)[i++],
					&g->single_quotes, &g->double_quotes); // tırnakları geç dolara kadar
			tmp = check_malloc(append_literal(tmp, *value, &start, &i)); // tmp'ye ekle
		}
	}
	return (*value = tmp, 1); // value'ya tmp'yi ata
}
