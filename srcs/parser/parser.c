/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:48:08 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/21 14:27:18 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static void	init_value(t_command *cmd)
{
	cmd->p_type = 0;
	cmd->fd[0] = STDIN_FILENO;
	cmd->fd[1] = STDOUT_FILENO;
	cmd->pid = -1;
	cmd->the_fd = -1;
	cmd->rds = NULL;
	cmd->next = NULL;
	cmd->is_work = 1;
}

static t_command	*cmd_init(t_list *token)
{
	t_command	*cmd;
	size_t		i;

	if (!token)
		return (NULL);
	cmd = check_malloc(ft_calloc(1, sizeof(t_command)));
	// tokenleri pipe'a kadar alır ve komut oluşturur
	if (identifier(token->content) == PIPE)
		token = token->next;
	i = token_len(token);
	cmd->value = (char **)check_malloc(ft_calloc((i + 1), sizeof(char *)));
	cmd->value[i] = NULL;
	init_value(cmd); // komutun değerlerini başlatır
	return (cmd);
}

static int	get_token(t_command **cmd, t_list **t, size_t *i)
{
	if (!rdr_position(*cmd))
		return (0);
	*i = 0;
	(*cmd)->next = cmd_init(*t);
	if (!(*cmd)->next)
		return (0);
	(*cmd)->p_type = 2;
	*cmd = (*cmd)->next;
	(*cmd)->p_type = 1;
	return (1);
}

static int	get_command(t_command **cmd, t_list **t, size_t *i)
{
    int		d;          // Çift tırnak durumu
    int		s;          // Tek tırnak durumu
    size_t	j;          // Döngü sayacı
    size_t	start;      // Kelimenin başlangıç indeksi
    char	*cleanstr;  // Temizlenmiş string

    j = 0;
    d = 0;
    s = 0;
    cleanstr = (char *)(*t)->content;  // Token içeriğini al
	// echo "merhaba dostlar 23" komutunu echo ve "merhaba dostlar 23" olarak ayırır value dizisine ekler
    while (cleanstr[j])  // Stringin sonuna kadar döngü
    {
        while (cleanstr[j] && is_white_space(cleanstr[j]) && !d && !s)  // Boşluk karakterlerini atla
            j++;
        start = j;  // Kelimenin başlangıç indeksini belirle
        if (cleanstr[j])  // Eğer hala karakter varsa
        {
            j = word_end(cleanstr, j, &d, &s);  // Kelimenin sonunu belirle
            cleanstr = check_malloc(ft_substr(cleanstr, start, j - start));  // Kelimeyi ayır ve bellekte yer ayır
            (*cmd)->value[(*i)++] = cleanstr;  // Komut yapısına kelimeyi ekle
            cleanstr = (char *)(*t)->content;  // Token içeriğini tekrar al
        }
    }
    return (1);  // Başarılı dönüş
}

void	parser(t_global *g)
{
	size_t		i;
	t_command	*cmd;
	t_list		*token;

	if (g->control == 0)
		return ;
	i = 0;
	token = g->token_list;
	// komut listesi için token listesinden komutları ayırır
	cmd = cmd_init(token);
	g->cmd_list = cmd;
	while (token)
	{
		if (identifier(token->content) != PIPE) // pipe değilse tokenları alır
			get_command(&cmd, &token, &i); // tokenları alır ve value dizisine ekler ["echo", "merhaba dostlar 23"] gibi
		else
			get_token(&cmd, &token, &i); // pipe ise komutları birbirine bağlar ve bir sonraki cmd geçer
		token = token->next; // bir sonraki tokena geçer
	}
	rdr_position(cmd); // yönlendirme operatörlerini kontrol eder ve ayırır
	if (!heredocs(g, g->cmd_list)) // heredoc varsa işler
		g->control = 0; // heredoc işlemi başarısız ise kontrolü kapatır diğer komutları yapmaz
}
