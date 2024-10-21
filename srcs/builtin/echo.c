/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:46:39 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/21 12:35:39 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	counter(size_t *i, size_t *j, int *nl, t_command *cmd)
{
    (*i) = 1;  // İndeks değişkeni i'yi 1 olarak başlat
    (*nl) = 1; // Yeni satır bayrağını 1 olarak başlat (yeni satır eklenecek)

    // Eğer komutun değeri varsa ve ilk karakter '-' ve ikinci karakter 'n' ise
    if (cmd->value[*i] && cmd->value[*i][0] == '-' && cmd->value[*i][1] == 'n')
    {
        // Komutun değeri boyunca döngü
        while (cmd->value[*i] && cmd->value[*i][0] == '-')
        {
            (*j) = 1; // İndeks değişkeni j'yi 1 olarak başlat
			            (*j) = 1; // İndeks değişkeni j'yi 1 olarak başlat
            // Komutun değerindeki karakterler 'n' olduğu sürece döngü
            while (cmd->value[*i][*j] == 'n')
                (*j)++; // j'yi artır
            // Eğer komutun değerinin sonuna gelindiyse
            if (cmd->value[*i][*j] == '\0')
                (*nl) = 0; // Yeni satır bayrağını 0 yap (yeni satır eklenmeyecek)
            else
                break ; // Döngüden çık

            (*i)++; // i'yi artır
        }
    }
}

int	echo(t_command *c, t_global *g)
{
	size_t	i;
	size_t	j;
	int		nline;

	counter(&i, &j, &nline, c);
	if (c->value[i])
		ft_putstr_fd(c->value[i++], c->fd[1]);
	while (c->value[i])
	{
		ft_putstr_fd(" ", c->fd[1]); // her kelime başına boşluk ekler
		ft_putstr_fd(c->value[i++], c->fd[1]); //kelimeyi yazdırır
	}
	if (nline) // newline varsa yeni satır ekle
		ft_putstr_fd("\n", c->fd[1]);
	return (g->error_no = 0, 1);
}
