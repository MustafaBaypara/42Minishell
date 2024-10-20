/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 15:12:50 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/20 19:06:39 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	space_cleaner(t_global *g, t_list *prev)
{
    t_list	*tmp;   // Geçici değişken
    t_list	*head;  // Listenin başı

    tmp = g->token_list;  // Token listesinin başını al
    head = g->token_list; // Listenin başını sakla
    while (tmp)  // Listenin sonuna kadar döngü
    {
        if (!check_space((char *)tmp->content))  // Eğer token boşluksa
        {
            if (head == tmp)  // Eğer bu düğüm listenin başıysa
                g->token_list = tmp->next;  // Listenin başını güncelle
            else
                prev->next = tmp->next;  // Önceki düğümün sonraki işaretçisini güncelle
            remove_from_list(&g->garbage_list, tmp);  // Düğümü garbage listesinden çıkar
            if (prev)
                tmp = prev->next;  // Geçici değişkeni güncelle
            else
                tmp = g->token_list;  // Geçici değişkeni güncelle
        }
        else
        {
            prev = tmp;  // Önceki düğümü güncelle
            tmp = tmp->next;  // Geçici değişkeni güncelle
        }
    }
}

static int	check_quote(t_list *token, int sq, int dq)
{
    size_t	q_double;  // Çift tırnak sayacı
    size_t	q_single;  // Tek tırnak sayacı
    size_t	i;         // Döngü sayacı
    char	*content;  // Token içeriği

    i = -1;
    q_double = 0;
    q_single = 0;
    content = (char *)token->content;  // Token içeriğini al

    while (content[++i])  // İçeriği karakter karakter dolaş
    {
        if (content[i] == '\'' && !dq)  // Eğer karakter tek tırnaksa ve çift tırnak içinde değilsek
        {
            q_single++;  // Tek tırnak sayacını artır
            sq = !sq;    // Tek tırnak durumunu değiştir
        }
        if (content[i] == '"' && !sq)  // Eğer karakter çift tırnaksa ve tek tırnak içinde değilsek
        {
            q_double++;  // Çift tırnak sayacını artır
            dq = !dq;    // Çift tırnak durumunu değiştir
        }
    }

    // Eğer çift tırnak veya tek tırnak sayısı çift değilse, hata döndür
    if (q_double % 2 != 0 || q_single % 2 != 0)
        return (0);

    return (1);  // Her şey yolundaysa 1 döndür
}

static int	check_pipe(t_list *token, size_t i)
{
	// kontrol ettiği komut örneği : ls | | ls
    if (identifier(token->content) != PIPE)  // Eğer token PIPE değilse
        return (1);  // Her şey yolunda, 1 döndür

    if (token->next == NULL)  // Eğer bu token son token ise
        return (0);  // Hata, 0 döndür

    else if (identifier(token->next->content) == PIPE)  // Eğer bir sonraki token da PIPE ise
        return (0);  // Hata, 0 döndür

    else if (i == 0 && token->next != NULL)  // Eğer bu ilk token ise ve bir sonraki token varsa
        return (0);  // Hata, 0 döndür

    return (1);  // Her şey yolunda, 1 döndür
}

static int	check_rdr(t_list *token, size_t i)
{
	// kontrol ettiği komut örneği : ls > > asd.txt
    t_token_types	type;

    type = identifier(token->content);  // Token içeriğinin türünü belirle
    if (type != RDR_IN && type != RDRD_IN && type != RDR_OUT && type != HEREDOC)
        return (1);  // Eğer token yönlendirme türünde değilse, 1 döndür

    if (token->next)
        type = identifier(token->next->content);  // Bir sonraki token'in türünü belirle

    if (token->next == NULL) // echo asd > | örneği
        return (0);  // Eğer bu son token ise ve yönlendirme türündeyse, 0 döndür
    else if (identifier(token->next->content) == PIPE) // echo < |
        return (0);  // Eğer bir sonraki token PIPE ise, 0 döndür
    else if (type == RDR_IN || type == RDRD_IN
        || type == RDR_OUT || type == HEREDOC) // echo < >> asd.txt
        return (0);  // Eğer bir sonraki token yönlendirme türündeyse, 0 döndür
    else if (i == 0 && token->next == NULL)  
        return (0);  // Eğer bu ilk token ise ve bir sonraki token yoksa, 0 döndür

    return (1);  // Her şey yolunda, 1 döndür
}

int	check_syntax(t_global *g)
{
	t_list	*token;
	size_t	i;
	size_t	j;
	// space cleaner fonksiyonu ile gereksiz boşlukları token listesinden temizler
	space_cleaner(g, NULL);
	token = g->token_list;
	i = ft_lstsize(token);
	j = i;
	while (token) // Pipe ve redirection kontrolü yapar
	{
        if (!check_quote(token, 0, 0)) // Tek ve çift tırnak kontrolü yapar
		    return (ft_putstr_fd("syntax error near unexpected quote.\n", 2), 0); // Hata mesajı verir
		if (!check_pipe(token, i - j)) // Pipe kontrolü yapar
			return (ft_putstr_fd("syntax error near unexpected token.\n", 2),
				0);
		else if (!check_rdr(token, i - j)) // Redirection kontrolü yapar
			return (ft_putstr_fd("syntax error near unexpected token.\n", 2),
				0);
		j--; // Token konumunu belirleme amacıyla sayacı azaltır
		token = token->next;
	}
	return (1);
}
